#include "pch.h"
#include "CBugBossScript.h"

#include "CCameraPlayerTrackingScript.h"
#include "CAniFinishDestroyScript.h"
#include "CHPScript.h"
#include "CBossHPScript.h"
#include "CPlayerScript.h"
#include "CSwingObjScript.h"

#include "CSymbolScript.h"
#include "CColorBugScript.h"
#include "CLightBallScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <random>

CBugBossScript::CBugBossScript()
	: CScript(UINT(SCRIPT_TYPE::BUGBOSSSCRIPT))
	, m_CurLevel(nullptr)
	, m_LightObj(nullptr)
	, m_WhiteObj(nullptr)
	, m_WingObj(nullptr)
	, m_Light1(nullptr)
	, m_Light2(nullptr)
	, m_Light3(nullptr)
	, m_ELight1(nullptr)
	, m_ELight2(nullptr)
	, m_AttackLine(nullptr)
	, m_Speed(1200.f)
	, m_Phase(BugBossPhase::None)
	, m_PhaseIn(false)
	, m_ProductionTime(0.f)
	, m_PhaseTime(0.f)
	, m_IsAttack(false)
	, m_AttackCount(0)
	, m_AttackColorType(ColorBugType::Blue)
	, m_IsDown(false)
	, m_Phase1Attack0_Obj(nullptr)
	, m_AttackCooldown(3.f)
	, m_HpScript(nullptr)
	, m_Hit(false)
	, m_SaveHitTime(0.f)
	, m_InvincibilityTime(0.8f)
	, m_LotusTime(0.f)
	, m_LotusCount(0)
	, m_Effect(false)
	, m_Dead(false)
{
}

CBugBossScript::~CBugBossScript()
{
}

void CBugBossScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();

	m_CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_Player = m_CurLevel->FindObjectByName(L"Player");

	CGameObject* hpFrame = m_CurLevel->FindObjectByName(L"BHP");
	for (auto i : hpFrame->GetChildren())
	{
		if (wcscmp(i->GetName().c_str(), L"HPBar") == 0)
		{
			m_HPBar = i;
		}
	}

	m_HpScript = dynamic_cast<CHPScript*>(GetOwner()->GetScript("CHPScript"));

	for (auto i : GetOwner()->GetChildren())
	{
		if (wcscmp(i->GetName().c_str(), L"Light") == 0)
		{
			m_LightObj = i;
		}
		if (wcscmp(i->GetName().c_str(), L"White") == 0)
		{
			m_WhiteObj = i;
		}
		if (wcscmp(i->GetName().c_str(), L"Wing") == 0)
		{
			m_WingObj = i;
		}

		if (wcscmp(i->GetName().c_str(), L"ELight1") == 0)
		{
			m_ELight1 = i;
		}
		if (wcscmp(i->GetName().c_str(), L"ELight2") == 0)
		{
			m_ELight2 = i;
		}
	}

	m_Light1 = m_CurLevel->FindObjectByName(L"light1");
	m_Light2 = m_CurLevel->FindObjectByName(L"light2");
	m_Light3 = m_CurLevel->FindObjectByName(L"light3");

	FlipPlay((int)BugBossAni::Appeared, 0, false);

	// ó���� wing�� ������ �ʴ´�.
	m_WingObj->FlipBookComponent()->ReversePlay((int)BugBossAni::Dead - (int)BugBossAni::Wing, 0, false);

	m_LightObj->FlipBookComponent()->AddAlpha(0.3f);
	m_LightObj->FlipBookComponent()->AddColor(true, Vec3(1.5f, 1.5f, 1.5f));

	// ������ ���
	m_Phase1Attack0_Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\LightBall.pref");
	m_Phase1Attack1_Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\BugBall.pref");
}

void CBugBossScript::Tick()
{
	// ���� �� ���(�׽�Ʈ��)
	if (KEY_TAP(KEY::P))
	{
		Hit();
	}

	if (m_Dead)
	{
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			DeleteObject(GetOwner());
		}
		return;
	}

	if (m_Phase == BugBossPhase::Appeared)
	{
		m_ProductionTime += DT;
		Appeared();
	}

	if (m_Phase == BugBossPhase::Phase1)
	{
		m_PhaseTime += DT;
		Phase1();
		// ������� �� ���� ���� ���İ�... ���缭...

		// 2������ ��ȯ
		if (m_HpScript->GetHP() <= 23)
		{
			if (!m_IsAttack)
			{
				m_Phase = BugBossPhase::Phase2;
				m_PhaseIn = true;
				m_PhaseTime = 0.f;
				m_AttackCount = 0;

				m_LightObj->FlipBookComponent()->AddColor(false);
			}
		}
	}

	if (m_Phase == BugBossPhase::Phase2)
	{
		m_PhaseTime += DT;
		Phase2();

		// 3������ ��ȯ
		if (m_HpScript->GetHP() <= 12)
		{
			if (!m_IsAttack)
			{
				m_Phase = BugBossPhase::Phase3;
				m_PhaseIn = true;
			}
		}
		m_ProductionTime = 0.f;
	}

	if (m_Phase == BugBossPhase::Phase3)
	{
		m_PhaseTime += DT;

		if (m_PhaseIn)
		{
			m_LotusTime += DT;
			m_ProductionTime += DT;

			Phase3Production();
		}

		Phase3();
	}

	// �¾��� ��
	if (m_Hit)
	{
		m_SaveHitTime += DT;

		if (m_SaveHitTime >= m_InvincibilityTime)
		{
			m_Hit = false;
			m_SaveHitTime = 0.f;

			GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
			GetOwner()->FlipBookComponent()->AddAlpha(1.f);
			GetOwner()->FlipBookComponent()->AddColor(false);
		}
	}
}

void CBugBossScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugBossScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// �÷��̾�� Hit ������
	if (_OtherObject->GetLayerIdx() == 3 && !m_IsDown)
	{
		// Down ������ ���� �÷��̾�� Ÿ�� �Ұ���
		if (_OtherObject->GetScript("CPlayerScript") != nullptr)
		{
			CPlayerScript* script = dynamic_cast<CPlayerScript*>(_OtherObject->GetScript("CPlayerScript"));
			script->Hit();
		}
	}

	if (_OtherObject->GetLayerIdx() == 7)
	{
		// �ǰ� ������ ��
		if (m_IsDown)
		{
			CScript* script = _OtherObject->GetScript("CSwingObjScript");
			CSwingObjScript* pSwing = dynamic_cast<CSwingObjScript*>(script);
			if (pSwing)
			{
				if (pSwing->GetIsSwing() && !m_Hit)
				{
					Hit();
				}
			}
		}
	}
}

void CBugBossScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugBossScript::SaveToFile(FILE* _File)
{
}

void CBugBossScript::LoadFromFile(FILE* _File)
{
}

void CBugBossScript::FlipPlay(int _FliBookIdx, int _FPS, bool _Repeat)
{
	GetOwner()->FlipBookComponent()->Play(_FliBookIdx, _FPS, _Repeat);
	m_LightObj->FlipBookComponent()->Play(_FliBookIdx, _FPS, _Repeat);
	m_WhiteObj->FlipBookComponent()->Play(_FliBookIdx, _FPS, _Repeat);
	// ������ ���� ������ �������� ���ڸ� ����
	if (_FliBookIdx > (int)BugBossAni::StandAttack)
	{
		m_WingObj->FlipBookComponent()->Play(_FliBookIdx - (int)BugBossAni::Wing, _FPS, _Repeat);
	}
}

void CBugBossScript::FlipReversePlay(int _FliBookIdx, int _FPS, bool _Repeat)
{
	GetOwner()->FlipBookComponent()->ReversePlay(_FliBookIdx, _FPS, _Repeat);
	m_LightObj->FlipBookComponent()->ReversePlay(_FliBookIdx, _FPS, _Repeat);
	m_WhiteObj->FlipBookComponent()->ReversePlay(_FliBookIdx, _FPS, _Repeat);
	// ������ ���� ������ �������� ���ڸ� ����
	if (_FliBookIdx > (int)BugBossAni::StandAttack)
	{
		m_WingObj->FlipBookComponent()->ReversePlay(_FliBookIdx - (int)BugBossAni::Wing, _FPS, _Repeat);
	}
}

void CBugBossScript::Appeared()
{
	if (m_ProductionTime >= 5.f)
	{
		FlipPlay((int)BugBossAni::Appeared, 8, true);
		// ���� ��ƼŬ, ��¦ ��ƼŬ, �� �ֵθ��� ����Ʈ

		// ������ �����Ÿ��鼭 ��Ÿ��(���İ� ����?)

		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			// ������ 1�� �Ѿ
			m_Phase = BugBossPhase::Phase1;
			m_PhaseIn = true;

			// ���� HP�� Ȱ��ȭ
			CGameObject* hpFrame = m_CurLevel->FindObjectByName(L"BHP");
			CBossHPScript* hpScript = dynamic_cast<CBossHPScript*>(hpFrame->GetScript("CBossHPScript"));
			hpScript->Start();

			// ī�޶� ��Ŀ��
			CGameObject* mainCamera = m_CurLevel->FindObjectByName(L"MainCamera");
			CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
			CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);

			CGameObject* player = m_CurLevel->FindObjectByName(L"Player");
			cameraScript->Focus(player);

			FlipPlay((int)BugBossAni::Idle, 5, true);
		}

		// �� �� ����
		Vec3 eyesColor = Vec3(0.f, 0.f, 0.f);
		m_ELight1->Light2D()->SetLightColor(eyesColor);
		m_ELight2->Light2D()->SetLightColor(eyesColor);
	}
	else if (m_ProductionTime >= 4.f)
	{
		// ����Ʈ ����Ʈ Ȯ��
		float R = (140.f / 0.5f) * (m_ProductionTime - 4.f);
		if (R > 140.f)
		{
			R = 140.f;
		}
		float G = (145.f / 0.5f) * (m_ProductionTime - 4.f);
		if (G > 145.f)
		{
			G = 145.f;
		}
		float B = (200.f / 0.5f) * (m_ProductionTime - 4.f);
		if (B > 200.f)
		{
			B = 200.f;
		}
		Vec3 rgb = Vec3(R / 255.f, G / 255.f, B / 255.f);
		m_Light1->Light2D()->SetLightColor(rgb);
		m_Light2->Light2D()->SetLightColor(rgb);

		// �� �� ��
		float eyesR = 140.f - (140.f / 1.f) * (m_ProductionTime - 4.f);
		if (eyesR < 0.f)
		{
			eyesR = 0.f;
		}
		float eyesG = 145.f - (145.f / 1.f) * (m_ProductionTime - 4.f);
		if (eyesG < 0.f)
		{
			eyesG = 0.f;
		}
		float eyesB = 200.f - (200.f / 1.f) * (m_ProductionTime - 4.f);
		if (eyesB < 0.f)
		{
			eyesB = 0.f;
		}
		Vec3 eyesRGB = Vec3(eyesR / 255.f, eyesG / 255.f, eyesB / 255.f);
		m_ELight1->Light2D()->SetLightColor(eyesRGB);
		m_ELight2->Light2D()->SetLightColor(eyesRGB);
	}
	else if (m_ProductionTime >= 2.f)
	{
		float R = (140.f / 0.5f) * (m_ProductionTime - 2.f);
		if (R > 140.f)
		{
			R = 140.f;
		}
		float G = (145.f / 0.5f) * (m_ProductionTime - 2.f);
		if (G > 145.f)
		{
			G = 145.f;
		}
		float B = (200.f / 0.5f) * (m_ProductionTime - 2.f);
		if (B > 200.f)
		{
			B = 200.f;
		}
		// �� ���� ����Ʈ ����Ʈ
		Vec3 rgb = Vec3(R / 255.f, G / 255.f, B / 255.f);
		m_ELight1->Light2D()->SetLightColor(rgb);
		m_ELight2->Light2D()->SetLightColor(rgb);
	}
}

void CBugBossScript::Phase1()
{
	if (m_PhaseIn && m_PhaseTime >= 1.f)
	{
		// ���� ���� ���� (�ð��� ����� �õ尪 ���)
		static std::mt19937 engine(static_cast<unsigned int>(std::time(nullptr)));
		std::uniform_int_distribution<int> dist(0, 2);
		int random = dist(engine);

		Vec3 color;
		switch (random)
		{
		case 0:
			// �Ķ�
			color = Vec3(0.1f, 0.27f, 1.7f);
			break;
		case 1:
			// �ʷ�
			color = Vec3(0.f, 1.65f, 0.16f);
			break;
		case 2:
			// ����
			color = Vec3(3.f, 0.25f, 0.25f);
			break;
		default:
			break;
		}

		FlipPlay((int)BugBossAni::StandAttack, 8, false);
		ChargeEffect(color);
		m_LightObj->FlipBookComponent()->AddColor(true, color);
		m_LightObj->FlipBookComponent()->SetUseLight(false);

		m_PhaseIn = false;
	}

	if (m_PhaseTime >= 5.f)
	{
		if (m_AttackCount >= 1)
		{
			// �� ����
			m_AttackColor = Vec3(1.5f, 1.5f, 1.5f);
		}
		else
		{
			// �� ���� ����

			// ���� ���� ���� (�ð��� ����� �õ尪 ���)
			static std::mt19937 engine(static_cast<unsigned int>(std::time(nullptr)));
			std::uniform_int_distribution<int> dist(0, 2);
			int random = dist(engine);

			switch (random)
			{
			case 0:
				// �Ķ�
				m_AttackColor = Vec3(0.1f, 0.27f, 1.7f);
				break;
			case 1:
				// �ʷ�
				m_AttackColor = Vec3(0.f, 1.65f, 0.16f);
				break;
			case 2:
				// ����
				m_AttackColor = Vec3(3.f, 0.25f, 0.25f);
				break;
			default:
				break;
			}
		}

		FlipPlay((int)BugBossAni::StandAttack, 8, false);
		ChargeEffect(m_AttackColor);
		m_LightObj->FlipBookComponent()->AddColor(true, m_AttackColor);
		m_LightObj->FlipBookComponent()->SetUseLight(false);

		m_PhaseTime = 0.f;
	}

	if (GetOwner()->FlipBookComponent()->GetCurFlipBookIdx() == (int)BugBossAni::StandAttack)
	{
		// ���� ������
		if (GetOwner()->FlipBookComponent()->GetCurIdx() == 8 && !m_IsAttack)
		{
			if (m_AttackCount >= 1)
			{
				// �� ����
				Phase1Attack1();
				m_AttackCount = 0;
			}
			else
			{
				// �� ���� ����
				Phase1Attack0();
				m_AttackCount++;
			}
			m_IsAttack = true;
		}

		// ���� ����� ������ idle���·� �ǵ�����.
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			FlipPlay((int)BugBossAni::Idle, 5, true);
			m_LightObj->FlipBookComponent()->AddColor(true, Vec3(1.5f, 1.5f, 1.5f));
			m_LightObj->FlipBookComponent()->SetUseLight(true);
			m_IsAttack = false;
		}
	}
}

void CBugBossScript::Phase2()
{
	if (m_PhaseIn)
	{
		Phase2Production();
	}
	else
	{
		Phase23Attack();
	}
}

void CBugBossScript::Phase2Production()
{
	CGameObject* mainCamera = m_CurLevel->FindObjectByName(L"MainCamera");

	if (m_LotusObjs.size() == 0)
	{
		// ��Ŀ���� �ٽ� ������
		CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
		CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
		cameraScript->Focus(GetOwner());

		// light3 �ѱ�
		m_Light3->Light2D()->SetLightColor(Vec3(140.f / 255.f, 145.f / 255.f, 200.f / 255.f));

		// �������
		FlipPlay((int)BugBossAni::Wing, 8, false);
	}

	// ���� idle
	if (GetOwner()->FlipBookComponent()->GetCurFlipBookIdx() == (int)BugBossAni::Wing)
	{
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			FlipPlay((int)BugBossAni::WingIdle, 8, true);
		}
	}

	if (m_PhaseTime < 0.2f)
	{
		return;
	}

	// ���� ���� 6��
	if (m_PhaseTime >= 0.2f && m_LotusObjs.size() < 6)
	{
		// 45 90 135 225 270 315 ������ ��ȯ�Ѵ�.
		Ptr<CPrefab> lotus = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\LotusFlower.pref");
		// ������ ��ġ�� ��ȯ�Ѵ�.
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();
		float angles[6] = { 45.0f, 90.0f, 135.0f, 225.0f, 270.0f, 315.0f };
		float angle = angles[m_LotusObjs.size()] + 90.f;
		angle = fmod(angle, 360.0f);
		float X = pos.x + (150.f * cos(XMConvertToRadians(angle)));
		float Y = pos.y - (150.f * sin(XMConvertToRadians(angle)));
		m_LotusObjs.push_back(Instantiate(lotus, 0, Vec3(X, Y, 0.f), L"Lotus"));
		
		m_PhaseTime = 0.f;
		return;
	}

	// �� ���� ������ �ܾƿ�
	mainCamera->Camera()->SetScale(1.2f);
	// ���� �������ش�.
	CGameObject* directionalLight = m_CurLevel->FindObjectByName(L"DirectionalLight");
	directionalLight->Light2D()->SetLightColor(Vec3(45.f / 255.f, 45.f / 255.f, 60.f / 255.f));
	m_Light1->Light2D()->SetLightColor(Vec3(0.f, 0.f, 0.f));
	m_Light2->Light2D()->SetLightColor(Vec3(0.f, 0.f, 0.f));

	// ������ �ܰ����� ������.
	if (m_PhaseTime <= 1.5f)
	{
		for (int i = 0; i < 6; i++)
		{
			Vec3 pos = m_LotusObjs[i]->Transform()->GetRelativePos();
			float angles[6] = { 45.0f, 90.0f, 135.0f, 225.0f, 270.0f, 315.0f };
			float angle = angles[i] + 90.f;
			angle = fmod(angle, 360.0f);
			float X = pos.x + (480.f * DT * cos(XMConvertToRadians(angle)));
			float Y = pos.y - (480.f * DT * sin(XMConvertToRadians(angle)));
			m_LotusObjs[i]->Transform()->SetRelativePos(Vec3(X, Y, 0.f));
		}
	}

	// ���� ����
	if (m_PhaseTime >= 3.f)
	{
		m_PhaseIn = false;
		m_PhaseTime = 0.f;

		// ��Ŀ���� �ٽ� �÷��̾��
		CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
		CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
		CGameObject* player = m_CurLevel->FindObjectByName(L"Player");
		cameraScript->Focus(player);
	}
}

void CBugBossScript::Phase3()
{
	Phase23Attack();
}

void CBugBossScript::Phase3Production()
{
	// ���� ���ְ� ��Ӱ� �����
	// õõ�� ��Ӱ� �����
	CGameObject* directionalLight = m_CurLevel->FindObjectByName(L"DirectionalLight");
	float RG = 45.f + (0.f - 45.f) * ((m_ProductionTime - 1.f) / 1.5f);
	float B = 60.f + (0.f - 60.f) * ((m_ProductionTime - 1.f) / 1.5f);
	directionalLight->Light2D()->SetLightColor(Vec3(RG / 255.f, RG / 255.f, B / 255.f));

	m_Light3->Light2D()->SetLightColor(Vec3(45.f / 255.f, 45.f / 255.f, 80.f / 255.f));

	// ���� �ı�
	if (m_LotusTime >= 0.1f)
	{
		if (m_LotusCount < 6)
		{
			Vec3 pos = m_LotusObjs[m_LotusCount]->Transform()->GetRelativePos();
			// �ı� ����Ʈ

			DeleteObject(m_LotusObjs[m_LotusCount++]);

			m_LotusTime = 0.f;
		}
		else
		{
			m_LotusObjs.clear();
		}
	}

	if (m_ProductionTime > 1.5f)
	{
		// �÷��̾�� ����Ʈ ����Ʈ �޾��ֱ�
		m_Player->AddComponent(new CLight2D);
		m_Player->Light2D()->SetLightType(LIGHT_TYPE::POINT);
		m_Player->Light2D()->SetRadius(50.f);
		m_Player->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));

		m_PhaseIn = false;
	}
}

void CBugBossScript::Phase1Attack0()
{
	// LightBall 8�� ��ȯ
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	float angle = 195.f;
	// ������ ��ġ�� ��ȯ�Ѵ�.
	for (int i = 0; i < 8; i++)
	{
		float X = pos.x + (240.f * cos(XMConvertToRadians(angle)));
		float Y = pos.y + (240.f * sin(XMConvertToRadians(angle)));
		CGameObject* lightBall = Instantiate(m_Phase1Attack0_Obj, 10, Vec3(X, Y, 0.f), L"LightBall");

		lightBall->FlipBookComponent()->AddColor(true, m_AttackColor);
		float setAngle = ((angle / 180.f) * XM_PI) + ((270.f / 180.f) * XM_PI);
		lightBall->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, setAngle));

		angle += 20.f;
	}
}

void CBugBossScript::Phase1Attack1()
{
	// BugBall 8�� ��ȯ
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	float angle = 195.f;
	// ������ ��ġ�� ��ȯ�Ѵ�.
	for (int i = 0; i < 8; i++)
	{
		float X = pos.x + (240.f * cos(XMConvertToRadians(angle)));
		float Y = pos.y + (240.f * sin(XMConvertToRadians(angle)));
		CGameObject* lightBall = Instantiate(m_Phase1Attack1_Obj, 10, Vec3(X, Y, 0.f), L"BugBall");

		float setAngle = ((angle / 180.f) * XM_PI) + ((270.f / 180.f) * XM_PI);
		lightBall->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, setAngle));

		angle += 20.f;
	}
}

void CBugBossScript::Phase23Attack()
{
	// ���� ���� ���� �� 
	if (m_IsAttack)
	{
		// �Ʒ� �������� ����
		Vec3 down = -1.f * GetOwner()->Transform()->GetRelativeDir(DIR::UP);
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();

		pos += m_Speed * DT * down;
		pos.z = -2000.f;
		GetOwner()->Transform()->SetRelativePos(pos);
	}
	
	// Down
	if (!m_IsAttack && m_IsDown && m_IsDownOK)
	{
		// ���ư��� ���� ����
		if (m_PhaseTime <= 0.6f)
		{
			Vec3 down = -1.f * GetOwner()->Transform()->GetRelativeDir(DIR::UP);
			Vec3 pos = GetOwner()->Transform()->GetRelativePos();

			pos += m_Speed * DT * down;
			pos.z = -2000.f;
			GetOwner()->Transform()->SetRelativePos(pos);
		}
		else if (m_PhaseTime <= 5.6f)
		{
			// 5�� ���� ���� ����
			GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
			m_LightObj->FlipBookComponent()->AddColor(false);
			m_WingObj->FlipBookComponent()->AddColor(false);
			FlipPlay((int)BugBossAni::Down, 8, false);

			// Down Effect
			if (GetOwner()->FlipBookComponent()->GetIsFinish())
			{
				if (!m_Effect)
				{
					Ptr<CPrefab> downEffect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\DownEffect.pref");
					Instantiate(downEffect, 0, GetOwner()->Transform()->GetRelativePos(), L"DownEffect");
					m_Effect = true;
				}
			}

			// AttackLine ����
			if (m_AttackLine != nullptr)
			{
				DeleteObject(m_AttackLine);
				m_AttackLine = nullptr;
			}

			// �� ���¿��� HP�� 12 ���ϸ� 3������ ����
			if (m_HpScript->GetHP() <= 12 && m_Phase == BugBossPhase::Phase2)
			{
				m_Phase = BugBossPhase::Phase3;
				m_PhaseIn = true;
				m_AttackCooldown = 1.f;
			}
		}
		else
		{
			// �ٿ� ���� ����
			m_PhaseTime = 0.f;
			m_IsDown = false;
			m_IsDownOK = false;
			m_AttackCount = -1;
			m_Effect = false;
			FlipReversePlay((int)BugBossAni::Down, 8, false);
		}

		return;
	}

	// ���� ���� �غ�
	if (GetOwner()->FlipBookComponent()->GetCurFlipBookIdx() == (int)BugBossAni::WingAttackReady)
	{
		// ���� ���� ����
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			FlipPlay((int)BugBossAni::WingAttack, 8, false);
			m_IsAttack = true;
			// ī�޶� ��鸲
			CGameObject* mainCamera = m_CurLevel->FindObjectByName(L"MainCamera");
			CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
			CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
			cameraScript->Shaking();
		}
	}

	if (m_AttackLine != nullptr)
	{
		if (m_IsAttack)
		{
			if (m_PhaseTime >= 1.f)
			{
				DeleteObject(m_AttackLine);
				m_AttackLine = nullptr;
			}
			else if (m_PhaseTime >= 0.5f)
			{
				float curAlpha = 0.5f + (0.f - 0.5f) * ((m_PhaseTime - 0.5f) / 0.5f);
				m_AttackLine->SpriteComponent()->AddAlpha(curAlpha);
			}
		}
	}

	// Down�� ������ �ٽ� �߾�����
	if (m_AttackCount == -1)
	{
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();
		// Down�� �� ������ ���� �ö�
		if (GetOwner()->FlipBookComponent()->GetCurFlipBookIdx() == (int)BugBossAni::Down)
		{	
			// �ٽ� WingIdle ���·� �ٲ���
			if (GetOwner()->FlipBookComponent()->GetIsFinish())
			{
				FlipPlay((int)BugBossAni::WingIdle, 8, true);
			}

			return;
		}

		if (m_PhaseTime <= 1.5f)
		{
			// ���� �ö󰡴� �ڵ�
			Vec3 up = GetOwner()->Transform()->GetRelativeDir(DIR::UP);
			pos += m_Speed * up * DT;
			GetOwner()->Transform()->SetRelativePos(pos);
		}
		else if (m_PhaseTime <= 1.6f)
		{
			// �߾����� �̵�
			GetOwner()->Transform()->SetRelativePos(Vec3(0.f, pos.y, pos.z));
			m_SavePos2 = GetOwner()->Transform()->GetRelativePos();
		}
		// �ٽ� ����� ��ġ�� �̵�
		else if (m_PhaseTime <= 2.6f)
		{
			pos = m_SavePos2 + (m_SavePos - m_SavePos2) * ((m_PhaseTime - 1.6f) / 1.f);
			GetOwner()->Transform()->SetRelativePos(pos);
		}
		else
		{
			// ����� ��ġ�� ���� �ٽ� 0���� ����
			m_AttackCount = 0;
			m_PhaseTime = 0.f;
		}
		
		return;
	}

	if (m_AttackCount == 0 && m_PhaseTime >= 2.5f)
	{
		// �� ���� ��ȯ
		SpawnColorBugs();

		// ���� ��ġ ����
		m_SavePos = GetOwner()->Transform()->GetRelativePos();

		// �� ����
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(0, 2);
		int random = distrib(gen);

		CGameObject* symbol = new CGameObject;
		symbol->SetName(L"Symbol");
		symbol->AddComponent(new CTransform);
		symbol->AddComponent(new CMeshRender);
		symbol->AddComponent(new CSpriteComponent);
		symbol->AddComponent(new CSymbolScript);
		symbol->Transform()->SetRelativePos(m_SavePos);
		symbol->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 1.f));
		symbol->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
		symbol->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
		switch (random)
		{
		case 0:
		{
			// �Ķ�
			m_AttackColor = Vec3(0.1f, 0.27f, 1.7f);
			m_AttackColorType = ColorBugType::Blue;

			Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\boss\\symbol\\Blue.sprite");
			symbol->SpriteComponent()->AddSprite(pSprite);
		}
			break;
		case 1:
		{
			// �ʷ�
			m_AttackColor = Vec3(0.f, 1.65f, 0.16f);
			m_AttackColorType = ColorBugType::Green;

			Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\boss\\symbol\\Green.sprite");
			symbol->SpriteComponent()->AddSprite(pSprite);
		}
			break;
		case 2:
		{
			// ����
			m_AttackColor = Vec3(3.f, 0.25f, 0.25f);
			m_AttackColorType = ColorBugType::Red;

			Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\boss\\symbol\\Red.sprite");
			symbol->SpriteComponent()->AddSprite(pSprite);
		}
			break;
		default:
			break;
		}
		symbol->SpriteComponent()->AddAlpha(0.7f);
		symbol->SpriteComponent()->SetUseLight(false);
		symbol->SpriteComponent()->AddColor(true, m_AttackColor);
		CreateObject(symbol, 0);

		m_LightObj->FlipBookComponent()->AddColor(true, m_AttackColor);
		m_WingObj->FlipBookComponent()->AddColor(true, m_AttackColor);
		FlipPlay((int)BugBossAni::WingAttackReady, 8, false);

		// ������ 3�� ��� LightBall 8�� ��ȯ
		if (m_Phase == BugBossPhase::Phase3)
		{
			Phase3SpawnLightBall();
		}

		m_AttackCount++;
		m_PhaseTime = 0.f;
		return;
	}

	if (m_PhaseTime >= 2.5f)
	{
		if (m_IsAttack)
		{
			// m_SavePos�� ������ �������� ��ȯ, �÷��̾ �ٶ󺸴� ����, ����
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> distrib(0, 359);

			float angle = distrib(gen) + 90.f;
			angle = fmod(angle, 360.0f);
			float X = m_SavePos.x + (1000.f * cos(XMConvertToRadians(angle)));
			float Y = m_SavePos.y - (1000.f * sin(XMConvertToRadians(angle)));
			GetOwner()->Transform()->SetRelativePos(Vec3(X, Y, -2000.f));

			Vec3 pos = GetOwner()->Transform()->GetRelativePos();
			Vec3 playerPos = m_Player->Transform()->GetRelativePos();

			Vec2 vDirection = Vec2(playerPos.x - pos.x, playerPos.y - pos.y);
			float angleZ = atan2(vDirection.y, vDirection.x);

			angleZ += XM_PIDIV2; // 90�� ����

			GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));

			m_AttackLine = new CGameObject;
			m_AttackLine->SetName(L"AttackLine");
			m_AttackLine->AddComponent(new CTransform);
			m_AttackLine->AddComponent(new CMeshRender);
			m_AttackLine->AddComponent(new CSpriteComponent);

			m_AttackLine->Transform()->SetRelativeScale(Vec3(1000.f, 3000.f, 1.f));
			m_AttackLine->Transform()->SetRelativePos(pos);
			m_AttackLine->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));

			m_AttackLine->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			m_AttackLine->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

			Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\boss\\effect\\WingAttackLine1.sprite");
			m_AttackLine->SpriteComponent()->AddSprite(pSprite);
			m_AttackLine->SpriteComponent()->AddColor(true, m_AttackColor);
			m_AttackLine->SpriteComponent()->SetUseLight(false);
			m_AttackLine->SpriteComponent()->AddAlpha(0.5f);

			CreateObject(m_AttackLine, 0);
			m_IsAttack = false;
		}
		else
		{
			if (m_IsDown)
			{
				m_IsDownOK = true;
				m_PhaseTime = 0.f;
			}
		}

		if (m_PhaseTime >= m_AttackCooldown)
		{
			m_IsAttack = true;
			m_PhaseTime = 0.f;
			m_IsDownOK = false;

			// ī�޶� ��鸲
			CGameObject* mainCamera = m_CurLevel->FindObjectByName(L"MainCamera");
			CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
			CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
			cameraScript->Shaking();
		}
	}
}

void CBugBossScript::SpawnColorBugs()
{
	Ptr<CPrefab> blue = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ColorBug_Blue.pref");
	Ptr<CPrefab> green = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ColorBug_Green.pref");
	Ptr<CPrefab> red = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ColorBug_Red.pref");

	// ���� ������ ��ġ�� ��ȯ�Ѵ�.
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 359);

	float angle = distrib(gen) + 90.f;
	angle = fmod(angle, 360.0f);
	float X = pos.x + (350.f * cos(XMConvertToRadians(angle)));
	float Y = pos.y - (350.f * sin(XMConvertToRadians(angle)));
	m_LotusObjs.push_back(Instantiate(blue, 0, Vec3(X, Y, 0.f), L"ColorBug_Blue"));

	angle = distrib(gen) + 90.f;
	angle = fmod(angle, 360.0f);
	X = pos.x + (350.f * cos(XMConvertToRadians(angle)));
	Y = pos.y - (350.f * sin(XMConvertToRadians(angle)));
	m_LotusObjs.push_back(Instantiate(green, 0, Vec3(X, Y, 0.f), L"ColorBug_Green"));

	angle = distrib(gen) + 90.f;
	angle = fmod(angle, 360.0f);
	X = pos.x + (350.f * cos(XMConvertToRadians(angle)));
	Y = pos.y - (350.f * sin(XMConvertToRadians(angle)));
	m_LotusObjs.push_back(Instantiate(red, 0, Vec3(X, Y, 0.f), L"ColorBug_Red"));
}

void CBugBossScript::Phase3SpawnLightBall()
{
	// ���� �� �����ؼ� �� 4���� LightBall �� 8�� ��ȯ
	Vec3 color1;
	Vec3 color2;
	switch (m_AttackColorType)
	{
	case ColorBugType::Blue:
		color1 = Vec3(3.f, 0.25f, 0.25f); // ����
		color2 = Vec3(0.f, 1.65f, 0.16f); // �ʷ�
		break;
	case ColorBugType::Green:
		color1 = Vec3(0.f, 1.65f, 0.16f); // �Ķ�
		color2 = Vec3(3.f, 0.25f, 0.25f); // ����
		break;
	case ColorBugType::Red:
		color1 = Vec3(0.f, 1.65f, 0.16f); // �ʷ�
		color2 = Vec3(0.f, 1.65f, 0.16f); // �Ķ�
		break;
	default:
		break;
	}

	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	float angle = 0.f;
	for (int i = 0; i < 4; i++)
	{
		float X = pos.x + (240.f * cos(XMConvertToRadians(angle)));
		float Y = pos.y + (240.f * sin(XMConvertToRadians(angle)));
		CGameObject* lightBall = Instantiate(m_Phase1Attack0_Obj, 10, Vec3(X, Y, 0.f), L"LightBall");
		lightBall->FlipBookComponent()->AddColor(true, color1);
		float setAngle = ((angle / 180.f) * XM_PI) + ((270.f / 180.f) * XM_PI);
		// 		setAngle += XM_PIDIV2; // 90�� ����
		lightBall->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, setAngle));
		CLightBallScript* script = dynamic_cast<CLightBallScript*>(lightBall->GetScript("CLightBallScript"));
		script->SetDeadOK(false);
		m_LightBallObjs.push_back(lightBall);

		angle += 45.f;
	}

	for (int i = 0; i < 4; i++)
	{
		float X = pos.x + (240.f * cos(XMConvertToRadians(angle)));
		float Y = pos.y + (240.f * sin(XMConvertToRadians(angle)));
		CGameObject* lightBall = Instantiate(m_Phase1Attack0_Obj, 10, Vec3(X, Y, 0.f), L"LightBall");

		lightBall->FlipBookComponent()->AddColor(true, color2);
		float setAngle = ((angle / 180.f) * XM_PI) + ((270.f / 180.f) * XM_PI);
		lightBall->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, setAngle));
		CLightBallScript* script = dynamic_cast<CLightBallScript*>(lightBall->GetScript("CLightBallScript"));
		script->SetDeadOK(false);
		m_LightBallObjs.push_back(lightBall);

		angle += 45.f;
	}
}

void CBugBossScript::ChargeEffect(Vec3 _Color)
{
	CGameObject* charge = new CGameObject;
	charge->SetName(L"Charge");
	charge->AddComponent(new CTransform);
	charge->AddComponent(new CMeshRender);
	charge->AddComponent(new CFlipBookComponent);
	charge->AddComponent(new CAniFinishDestroyScript);

	charge->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	charge->Transform()->SetRelativeScale(Vec3(500, 500, -500));
	
	charge->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	charge->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	Ptr<CFlipBook> pFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Boss\\Effect\\Charge.flip");
	charge->FlipBookComponent()->AddFlipBook(0, pFlip);
	charge->FlipBookComponent()->Play(0, 8, false);
	//charge->FlipBookComponent()->SetUseLight(false);

	CreateObject(charge, 0);

	charge = new CGameObject;
	charge->SetName(L"ChargeBulr");
	charge->AddComponent(new CTransform);
	charge->AddComponent(new CMeshRender);
	charge->AddComponent(new CFlipBookComponent);
	charge->AddComponent(new CAniFinishDestroyScript);

	charge->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	charge->Transform()->SetRelativeScale(Vec3(500, 500, -500));

	charge->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	charge->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	pFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Boss\\Effect\\Charge_Blur.flip");
	charge->FlipBookComponent()->AddFlipBook(0, pFlip);
	charge->FlipBookComponent()->Play(0, 8, false);
	charge->FlipBookComponent()->AddAlpha(0.5f);
	charge->FlipBookComponent()->AddColor(true, _Color);
	charge->FlipBookComponent()->SetUseLight(false);

	CreateObject(charge, 0);
}

void CBugBossScript::HitEffect()
{
	// ������ ����+���� �ð� ���� ������
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	GetOwner()->FlipBookComponent()->AddAlpha(0.3f);
	GetOwner()->FlipBookComponent()->AddColor(true, Vec3(1.f, 0.3f, 0.2f));
}

void CBugBossScript::Active()
{
	// ���� ����
	m_Phase = BugBossPhase::Appeared;
	// �� ��ο����� �����
	CGameObject* directionalLight = m_CurLevel->FindObjectByName(L"DirectionalLight");
	directionalLight->Light2D()->SetLightColor(Vec3(0, 0, 0));

	// ī�޶� ��Ŀ��
	CGameObject* mainCamera = m_CurLevel->FindObjectByName(L"MainCamera");
	CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
	CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
	cameraScript->Focus(GetOwner());
}

void CBugBossScript::Hit()
{
	if (m_Hit)
	{
		return;
	}

	// �ǰ� ����Ʈ
	HitEffect();

	m_Hit = true;

	// ������ �ֱ�
	if (m_HpScript != nullptr)
	{
		m_HpScript->Hit(1, m_HPBar);
	}
}

void CBugBossScript::Dead()
{
	// ����
	m_Dead = true;
	// ���� �ִϸ��̼� ��� �� ����
	FlipPlay((int)BugBossAni::Dead, 8, false);
}

void CBugBossScript::Phase23Down(ColorBugType _ColorType)
{
	// ���� ������ �ٿ����� �ٲ۴�.
	if (m_AttackColorType == _ColorType)
	{
		m_IsDown = true;

		// ������ �����ִ� �� ������ ������ ����
		CGameObject* colorBug = m_CurLevel->FindObjectByName(L"ColorBug_Blue");
		if (colorBug != nullptr)
		{
			DeleteObject(colorBug);
		}
		colorBug = m_CurLevel->FindObjectByName(L"ColorBug_Green");
		if (colorBug != nullptr)
		{
			DeleteObject(colorBug);
		}
		colorBug = m_CurLevel->FindObjectByName(L"ColorBug_Red");
		if (colorBug != nullptr)
		{
			DeleteObject(colorBug);
		}

		for (auto i : m_LightBallObjs)
		{
			CLightBallScript* script = dynamic_cast<CLightBallScript*>(i->GetScript("CLightBallScript"));
			script->Destroy();
		}
	}
}
