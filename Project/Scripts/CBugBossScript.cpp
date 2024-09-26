#include "pch.h"
#include "CBugBossScript.h"

#include "CCameraPlayerTrackingScript.h"
#include "CAniFinishDestroyScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <random>

CBugBossScript::CBugBossScript()
	: CScript(UINT(SCRIPT_TYPE::BUGBOSSSCRIPT))
	, m_LightObj(nullptr)
	, m_WhiteObj(nullptr)
	, m_WingObj(nullptr)
	, m_Light1(nullptr)
	, m_Light2(nullptr)
	, m_ELight1(nullptr)
	, m_ELight2(nullptr)
	, m_Phase(BugBossPhase::None)
	, m_PhaseIn(false)
	, m_AppearedTime(0.f)
	, m_Phase1Time(0.f)
	, m_IsAttack(false)
	, m_AttackCount(0)
	, m_Phase1Attack0_Obj(nullptr)
{
}

CBugBossScript::CBugBossScript(const CBugBossScript& _Origin)
	: CScript(_Origin)
	, m_LightObj(nullptr)
	, m_WhiteObj(nullptr)
	, m_WingObj(nullptr)
	, m_Light1(nullptr)
	, m_Light2(nullptr)
	, m_ELight1(nullptr)
	, m_ELight2(nullptr)
	, m_Phase(BugBossPhase::None)
	, m_PhaseIn(false)
	, m_AppearedTime(0.f)
	, m_Phase1Time(0.f)
	, m_IsAttack(false)
	, m_AttackCount(0)
	, m_Phase1Attack0_Obj(nullptr)
{
}

CBugBossScript::~CBugBossScript()
{
}

void CBugBossScript::Begin()
{
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

	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_Light1 = curLevel->FindObjectByName(L"light1");
	m_Light2 = curLevel->FindObjectByName(L"light2");

	FlipPlay((int)BugBossAni::Appeared, 0, false);

	// ó���� wing�� ������ �ʴ´�.
	m_WingObj->FlipBookComponent()->ReversePlay((int)BugBossAni::Dead - (int)BugBossAni::Wing, 0, false);

	m_LightObj->FlipBookComponent()->AddAlpha(0.3f);
	m_LightObj->FlipBookComponent()->AddColor(true, Vec3(1.5f, 1.5f, 1.5f));

	// ������ ���
	m_Phase1Attack0_Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\LightBall.pref");
}

void CBugBossScript::Tick()
{
	if (m_Phase == BugBossPhase::Appeared)
	{
		m_AppearedTime += DT;
		Appeared();
	}

	if (m_Phase == BugBossPhase::Phase1)
	{
		m_Phase1Time += DT;
		Phase1();
		// ������� �� ���� ���� ���İ�... ���缭...
	}
}

void CBugBossScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugBossScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
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

void CBugBossScript::Appeared()
{
	if (m_AppearedTime >= 5.f)
	{
		FlipPlay((int)BugBossAni::Appeared, 8, true);
		// ���� ��ƼŬ, ��¦ ��ƼŬ, �� �ֵθ��� ����Ʈ

		// ������ �����Ÿ��鼭 ��Ÿ��(���İ� ����?)

		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			// ������ 1�� �Ѿ
			m_Phase = BugBossPhase::Phase1;
			m_PhaseIn = true;

			// ī�޶� ��Ŀ��
			CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
			CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
			CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);

			CGameObject* player = curLevel->FindObjectByName(L"Player");
			cameraScript->Focus(player);

			FlipPlay((int)BugBossAni::Idle, 5, true);
		}

		// �� �� ����
		DeleteObject(m_ELight1);
		DeleteObject(m_ELight2);
	}
	else if (m_AppearedTime >= 4.f)
	{
		// ����Ʈ ����Ʈ Ȯ��
		float R = (140.f / 0.5f) * (m_AppearedTime - 4.f);
		if (R > 140.f)
		{
			R = 140.f;
		}
		float G = (145.f / 0.5f) * (m_AppearedTime - 4.f);
		if (G > 145.f)
		{
			G = 145.f;
		}
		float B = (200.f / 0.5f) * (m_AppearedTime - 4.f);
		if (B > 200.f)
		{
			B = 200.f;
		}
		Vec3 rgb = Vec3(R / 255.f, G / 255.f, B / 255.f);
		m_Light1->Light2D()->SetLightColor(rgb);
		m_Light2->Light2D()->SetLightColor(rgb);

		// �� �� ��
		float eyesR = 140.f - (140.f / 1.f) * (m_AppearedTime - 4.f);
		if (eyesR < 0.f)
		{
			eyesR = 0.f;
		}
		float eyesG = 145.f - (145.f / 1.f) * (m_AppearedTime - 4.f);
		if (eyesG < 0.f)
		{
			eyesG = 0.f;
		}
		float eyesB = 200.f - (200.f / 1.f) * (m_AppearedTime - 4.f);
		if (eyesB < 0.f)
		{
			eyesB = 0.f;
		}
		Vec3 eyesRGB = Vec3(eyesR / 255.f, eyesG / 255.f, eyesB / 255.f);
		m_ELight1->Light2D()->SetLightColor(eyesRGB);
		m_ELight2->Light2D()->SetLightColor(eyesRGB);
	}
	else if (m_AppearedTime >= 2.f)
	{
		float R = (140.f / 0.5f) * (m_AppearedTime - 2.f);
		if (R > 140.f)
		{
			R = 140.f;
		}
		float G = (145.f / 0.5f) * (m_AppearedTime - 2.f);
		if (G > 145.f)
		{
			G = 145.f;
		}
		float B = (200.f / 0.5f) * (m_AppearedTime - 2.f);
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
	if (m_PhaseIn && m_Phase1Time >= 1.f)
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

	if (m_Phase1Time >= 5.f)
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

		m_Phase1Time = 0.f;
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

void CBugBossScript::Phase1Attack0()
{
	// LightBall 8�� ��ȯ
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	float angle = 215.f;
	// ������ ��ġ�� ��ȯ�Ѵ�.
	for (int i = 0; i < 8; i++)
	{
		float X = pos.x + (260.f * cos(XMConvertToRadians(angle)));
		float Y = pos.y + (260.f * sin(XMConvertToRadians(angle)));
		CGameObject* lightBall = Instantiate(m_Phase1Attack0_Obj, 10, Vec3(X, Y, 0.f), L"LightBall");

		lightBall->FlipBookComponent()->AddColor(true, m_AttackColor);
		float setAngle = ((angle / 180.f) * XM_PI) + ((270.f / 180.f) * XM_PI);
		lightBall->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, setAngle));

		angle += 20.f;
	}
}

void CBugBossScript::Phase1Attack1()
{
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

void CBugBossScript::Active()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	// ���� ����
	m_Phase = BugBossPhase::Appeared;
	// �� ��ο����� �����
	CGameObject* directionalLight = curLevel->FindObjectByName(L"DirectionalLight");
	directionalLight->Light2D()->SetLightColor(Vec3(0, 0, 0));

	// ī�޶� ��Ŀ��
	CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
	CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
	CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
	cameraScript->Focus(GetOwner());
}
