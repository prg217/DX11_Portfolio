#include "pch.h"
#include "CBugBossScript.h"

#include "CCameraPlayerTrackingScript.h"
#include "CAniFinishDestroyScript.h"
#include "CHPScript.h"
#include "CBossHPScript.h"
#include "CPlayerScript.h"
#include "CSwingObjScript.h"

#include "CColorBugScript.h"

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
	, m_Light3(nullptr)
	, m_ELight1(nullptr)
	, m_ELight2(nullptr)
	, m_AttackLine(nullptr)
	, m_Speed(1200.f)
	, m_Phase(BugBossPhase::None)
	, m_PhaseIn(false)
	, m_AppearedTime(0.f)
	, m_PhaseTime(0.f)
	, m_IsAttack(false)
	, m_AttackCount(0)
	, m_AttackColorType(ColorBugType::Blue)
	, m_IsDown(false)
	, m_Phase1Attack0_Obj(nullptr)
	, m_HpScript(nullptr)
	, m_Hit(false)
	, m_SaveHitTime(0.f)
	, m_InvincibilityTime(0.4f)
{
}

CBugBossScript::~CBugBossScript()
{
}

void CBugBossScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();

	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_Player = curLevel->FindObjectByName(L"Player");

	CGameObject* hpFrame = curLevel->FindObjectByName(L"BHP");
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

	m_Light1 = curLevel->FindObjectByName(L"light1");
	m_Light2 = curLevel->FindObjectByName(L"light2");
	m_Light3 = curLevel->FindObjectByName(L"light3");

	FlipPlay((int)BugBossAni::Appeared, 0, false);

	// 처음엔 wing은 보이지 않는다.
	m_WingObj->FlipBookComponent()->ReversePlay((int)BugBossAni::Dead - (int)BugBossAni::Wing, 0, false);

	m_LightObj->FlipBookComponent()->AddAlpha(0.3f);
	m_LightObj->FlipBookComponent()->AddColor(true, Vec3(1.5f, 1.5f, 1.5f));

	// 프리팹 등록
	m_Phase1Attack0_Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\LightBall.pref");
	m_Phase1Attack1_Obj = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\BugBall.pref");
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
		m_PhaseTime += DT;
		Phase1();
		// 베리어는 볼 날릴 때만 알파값... 낮춰서...

		// 2페이즈 전환
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

		// 3페이즈 전환
		if (m_HpScript->GetHP() <= 12)
		{
			if (!m_IsAttack)
			{
				m_Phase = BugBossPhase::Phase3;
				m_PhaseIn = true;
			}
		}
	}

	// 맞았을 때
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
	// 플레이어에게 Hit 보내기
	if (_OtherObject->GetLayerIdx() == 3 && !m_IsDown)
	{
		// Down 상태일 때는 플레이어에게 타격 불가능
		if (_OtherObject->GetScript("CPlayerScript") != nullptr)
		{
			CPlayerScript* script = dynamic_cast<CPlayerScript*>(_OtherObject->GetScript("CPlayerScript"));
			script->Hit();
		}
	}

	if (_OtherObject->GetLayerIdx() == 7)
	{
		// 피격 가능할 때
		if (m_IsDown)
		{
			CScript* script = _OtherObject->GetScript("CSwingObjScript");
			CSwingObjScript* pSwing = dynamic_cast<CSwingObjScript*>(script);
			if (pSwing)
			{
				if (pSwing->GetIsSwing() && !m_Hit)
				{
					m_Hit = true;
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
	// 날개는 날개 나오기 전까지의 숫자를 빼기
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
		// 먼지 파티클, 반짝 파티클, 검 휘두르는 이펙트

		// 베리어 깜빡거리면서 나타남(알파값 조정?)

		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			// 페이즈 1로 넘어감
			m_Phase = BugBossPhase::Phase1;
			m_PhaseIn = true;

			// 보스 HP바 활성화
			CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CGameObject* hpFrame = curLevel->FindObjectByName(L"BHP");
			CBossHPScript* hpScript = dynamic_cast<CBossHPScript*>(hpFrame->GetScript("CBossHPScript"));
			hpScript->Start();

			// 카메라 포커스
			CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
			CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
			CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);

			CGameObject* player = curLevel->FindObjectByName(L"Player");
			cameraScript->Focus(player);

			FlipPlay((int)BugBossAni::Idle, 5, true);
		}

		// 눈 빛 삭제
		DeleteObject(m_ELight1);
		DeleteObject(m_ELight2);
	}
	else if (m_AppearedTime >= 4.f)
	{
		// 포인트 라이트 확장
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

		// 눈 빛 끔
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
		// 두 눈에 포인트 라이트
		Vec3 rgb = Vec3(R / 255.f, G / 255.f, B / 255.f);
		m_ELight1->Light2D()->SetLightColor(rgb);
		m_ELight2->Light2D()->SetLightColor(rgb);
	}
}

void CBugBossScript::Phase1()
{
	if (m_PhaseIn && m_PhaseTime >= 1.f)
	{
		// 난수 생성 엔진 (시간에 기반한 시드값 사용)
		static std::mt19937 engine(static_cast<unsigned int>(std::time(nullptr)));
		std::uniform_int_distribution<int> dist(0, 2);
		int random = dist(engine);

		Vec3 color;
		switch (random)
		{
		case 0:
			// 파랑
			color = Vec3(0.1f, 0.27f, 1.7f);
			break;
		case 1:
			// 초록
			color = Vec3(0.f, 1.65f, 0.16f);
			break;
		case 2:
			// 빨강
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
			// 볼 공격
			m_AttackColor = Vec3(1.5f, 1.5f, 1.5f);
		}
		else
		{
			// 빛 구슬 공격

			// 난수 생성 엔진 (시간에 기반한 시드값 사용)
			static std::mt19937 engine(static_cast<unsigned int>(std::time(nullptr)));
			std::uniform_int_distribution<int> dist(0, 2);
			int random = dist(engine);

			switch (random)
			{
			case 0:
				// 파랑
				m_AttackColor = Vec3(0.1f, 0.27f, 1.7f);
				break;
			case 1:
				// 초록
				m_AttackColor = Vec3(0.f, 1.65f, 0.16f);
				break;
			case 2:
				// 빨강
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
		// 공격 날리기
		if (GetOwner()->FlipBookComponent()->GetCurIdx() == 8 && !m_IsAttack)
		{
			if (m_AttackCount >= 1)
			{
				// 볼 공격
				Phase1Attack1();
				m_AttackCount = 0;
			}
			else
			{
				// 빛 구슬 공격
				Phase1Attack0();
				m_AttackCount++;
			}
			m_IsAttack = true;
		}

		// 공격 모션이 끝나면 idle상태로 되돌린다.
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
		Phase2Attack();
	}
}

void CBugBossScript::Phase2Production()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");

	if (m_LotusObjs.size() == 0)
	{
		// 포커스를 다시 보스로
		CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
		CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
		cameraScript->Focus(GetOwner());

		// light3 켜기
		m_Light3->Light2D()->SetLightColor(Vec3(140.f / 255.f, 145.f / 255.f, 200.f / 255.f));

		// 날개펴기
		FlipPlay((int)BugBossAni::Wing, 8, false);
	}

	// 날개 idle
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

	// 연꽃 생성 6개
	if (m_PhaseTime >= 0.2f && m_LotusObjs.size() < 6)
	{
		// 45 90 135 225 270 315 각도에 소환한다.
		Ptr<CPrefab> lotus = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\LotusFlower.pref");
		// 반지름 위치에 소환한다.
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

	// 다 생성 됐으면 줌아웃
	mainCamera->Camera()->SetScale(1.2f);
	// 빛을 조절해준다.
	CGameObject* directionalLight = curLevel->FindObjectByName(L"DirectionalLight");
	directionalLight->Light2D()->SetLightColor(Vec3(45.f / 255.f, 45.f / 255.f, 60.f / 255.f));

	// 연꽃을 외곽으로 보낸다.
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

	// 연출 끄기
	if (m_PhaseTime >= 3.f)
	{
		m_PhaseIn = false;
		m_PhaseTime = 0.f;

		// 포커스를 다시 플레이어로
		CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
		CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
		CGameObject* player = curLevel->FindObjectByName(L"Player");
		cameraScript->Focus(player);
	}
}

void CBugBossScript::Phase1Attack0()
{
	// LightBall 8개 소환
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	float angle = 195.f;
	// 반지름 위치에 소환한다.
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
	// BugBall 8개 소환
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	float angle = 195.f;
	// 반지름 위치에 소환한다.
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

void CBugBossScript::Phase2Attack()
{
	// 날기 공격 중일 때 
	if (m_IsAttack)
	{
		// 아래 방향으로 돌진
		Vec3 down = -1.f * GetOwner()->Transform()->GetRelativeDir(DIR::UP);
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();

		pos += m_Speed * DT * down;
		pos.z = -2000.f;
		GetOwner()->Transform()->SetRelativePos(pos);
	}
	
	// Down
	if (!m_IsAttack && m_IsDown)
	{
		// 랜덤 위치... 날아가다 주저 앉음
		if (m_PhaseTime <= 0.3f)
		{
			Vec3 down = -1.f * GetOwner()->Transform()->GetRelativeDir(DIR::UP);
			Vec3 pos = GetOwner()->Transform()->GetRelativePos();

			pos += m_Speed * DT * down;
			pos.z = -2000.f;
			GetOwner()->Transform()->SetRelativePos(pos);
		}
	}

	// 날기 공격 준비
	if (GetOwner()->FlipBookComponent()->GetCurFlipBookIdx() == (int)BugBossAni::WingAttackReady)
	{
		// 날기 공격 시작
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			FlipPlay((int)BugBossAni::WingAttack, 8, false);
			m_IsAttack = true;
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

	if (m_AttackCount == 0 && m_PhaseTime >= 2.5f)
	{
		// 색 벌레 소환
		SpawnColorBugs();

		// 기존 위치 저장
		m_SavePos = GetOwner()->Transform()->GetRelativePos();

		// 색 지정
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(0, 2);
		int random = distrib(gen);

		switch (random)
		{
		case 0:
			// 파랑
			m_AttackColor = Vec3(0.1f, 0.27f, 1.7f);
			m_AttackColorType = ColorBugType::Blue;
			// 색상 무늬 띄워야 함
			break;
		case 1:
			// 초록
			m_AttackColor = Vec3(0.f, 1.65f, 0.16f);
			m_AttackColorType = ColorBugType::Green;
			break;
		case 2:
			// 빨강
			m_AttackColor = Vec3(3.f, 0.25f, 0.25f);
			m_AttackColorType = ColorBugType::Red;
			break;
		default:
			break;
		}

		m_LightObj->FlipBookComponent()->AddColor(true, m_AttackColor);
		m_WingObj->FlipBookComponent()->AddColor(true, m_AttackColor);
		FlipPlay((int)BugBossAni::WingAttackReady, 8, false);

		m_AttackCount++;
		m_PhaseTime = 0.f;
		return;
	}

	if (m_PhaseTime >= 2.5f)
	{
		if (m_IsAttack)
		{
			// m_SavePos의 반지름 범위에서 소환, 플레이어를 바라보는 각도, 돌진
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

			angleZ += XM_PIDIV2; // 90도 보정

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
		

		if (m_PhaseTime >= 3.f)
		{
			m_IsAttack = true;
			m_PhaseTime = 0.f;
		}
	}
}

void CBugBossScript::SpawnColorBugs()
{
	Ptr<CPrefab> blue = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ColorBug_Blue.pref");
	Ptr<CPrefab> green = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ColorBug_Green.pref");
	Ptr<CPrefab> red = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\ColorBug_Red.pref");

	// 랜덤 반지름 위치에 소환한다.
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
}

void CBugBossScript::Active()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	// 연출 시작
	m_Phase = BugBossPhase::Appeared;
	// 방 어두워지게 만들고
	CGameObject* directionalLight = curLevel->FindObjectByName(L"DirectionalLight");
	directionalLight->Light2D()->SetLightColor(Vec3(0, 0, 0));

	// 카메라 포커스
	CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
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

	// 피격 이펙트
	HitEffect();

	m_Hit = true;

	// 빨갛게 변함+무적 시간 동안 반투명
	GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	GetOwner()->FlipBookComponent()->AddAlpha(0.3f);
	GetOwner()->FlipBookComponent()->AddColor(true, Vec3(1.f, 0.3f, 0.2f));

	// 데미지 주기
	if (m_HpScript != nullptr)
	{
		m_HpScript->Hit(1, m_HPBar);
	}
}

void CBugBossScript::Dead()
{
	// 죽음
}

void CBugBossScript::Phase2Down(ColorBugType _ColorType)
{
	// 다음 공격을 다운으로 바꾼다.
	if (m_AttackColorType == _ColorType)
	{
		m_IsDown = true;
	}
}
