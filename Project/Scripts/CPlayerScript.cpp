#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/enum.h>

#include "CCountDownDeleteScript.h"
#include "COguDancePointLightScript.h"

CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(200.f)
	, m_MinSpeed(200.f)
	, m_MaxSpeed(300.f)
	, m_IsRun(false)
	, m_MoveCount(0)
	, m_SaveFinalMoveTime(0)
	, m_SaveFinalDiagonalTime(0)
	, m_AllowedTime(0.1f)
	, m_CurMS(OguAniState::IDLE)
	, m_IdleDanceTime(8.f)
	, m_IsRunParticle(false)
{	
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Test", &m_Texture);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
}

void CPlayerScript::Tick()
{
	Move();

	if (TIME - m_SaveFinalMoveTime >= m_IdleDanceTime)
	{
		m_CurMS = OguAniState::IDLE_DANCE;
	}

	// A 버튼을 누르면 채를 휘두른다.
	if (KEY_TAP(KEY::A))
	{
		m_CurMS = OguAniState::SWING_DOWN;
	}

	// Q 버튼을 누르면 춤을 춘다.
	if (KEY_TAP(KEY::Q))
	{
		Dance();
	}

	AniState();
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	DeleteObject(_OtherObject);

	Vec3 vScale = Transform()->GetRelativeScale();

	vScale += Vec3(10.f, 10.f, 0.f);
	Collider2D()->SetScale(Collider2D()->GetScale() + Vec3(10.f, 10.f, 0.f));

	Transform()->SetRelativeScale(vScale);
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
	SaveAssetRef(m_Texture, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
	LoadAssetRef(m_Texture, _File);
}

void CPlayerScript::Move()
{
	Vec3 vPos = Transform()->GetRelativePos();

	// 마지막으로 움직인 시간
	if (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT) || KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::DOWN))
	{
		m_SaveFinalMoveTime = TIME;
	}	

	// 달리는 파티클
	if (KEY_PRESSED(KEY::LSHIFT) && !m_IsRunParticle)
	{
		if (KEY_TAP(KEY::LEFT) || KEY_TAP(KEY::RIGHT) || KEY_TAP(KEY::UP) || KEY_TAP(KEY::DOWN))
		{
			m_IsRunParticle = true;
			RunParticle();
		}
	}

	// Run
	if (KEY_TAP(KEY::LSHIFT))
	{
		m_IsRun = true;
		m_Speed = m_MaxSpeed;

		// 달리는 파티클
		if (!m_IsRunParticle)
		{
			if (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT) || KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::DOWN))
			{
				m_IsRunParticle = true;
				RunParticle();
			}
		}
	}
	if (KEY_RELEASED(KEY::LSHIFT))
	{
		m_IsRun = false;
		m_IsRunParticle = false;
		m_Speed = m_MinSpeed;
	}

	if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::UP))
	{
		if (m_IsRun)
		{
			m_CurMS = OguAniState::RUN_LEFTUP;
		}
		else
		{
			m_CurMS = OguAniState::WALK_LEFTUP;
		}

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::DOWN))
	{
		if (m_IsRun)
		{
			m_CurMS = OguAniState::RUN_LEFTDOWN;
		}
		else
		{
			m_CurMS = OguAniState::WALK_LEFTDOWN;
		}

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::UP))
	{
		if (m_IsRun)
		{
			m_CurMS = OguAniState::RUN_RIGHTUP;
		}
		else
		{
			m_CurMS = OguAniState::WALK_RIGHTUP;
		}

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::DOWN))
	{
		if (m_IsRun)
		{
			m_CurMS = OguAniState::RUN_RIGHTDOWN;
		}
		else
		{
			m_CurMS = OguAniState::WALK_RIGHTDOWN;
		}

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (TIME - m_SaveFinalDiagonalTime > m_AllowedTime)
	{
		if (KEY_PRESSED(KEY::LEFT))
		{
			if (m_IsRun)
			{
				m_CurMS = OguAniState::RUN_LEFT;
			}
			else
			{
				m_CurMS = OguAniState::WALK_LEFT;
			}
		}
		else if (KEY_PRESSED(KEY::RIGHT))
		{
			if (m_IsRun)
			{
				m_CurMS = OguAniState::RUN_RIGHT;
			}
			else
			{
				m_CurMS = OguAniState::WALK_RIGHT;
			}
		}
		else if (KEY_PRESSED(KEY::UP))
		{
			if (m_IsRun)
			{
				m_CurMS = OguAniState::RUN_UP;
			}
			else
			{
				m_CurMS = OguAniState::WALK_UP;
			}
		}
		else if (KEY_PRESSED(KEY::DOWN))
		{
			if (m_IsRun)
			{
				m_CurMS = OguAniState::RUN_DOWN;
			}
			else
			{
				m_CurMS = OguAniState::WALK_DOWN;
			}
		}

	}

	if (KEY_RELEASED(KEY::LEFT))
	{
		if (m_CurMS == OguAniState::WALK_LEFTUP || m_CurMS == OguAniState::RUN_LEFTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = OguAniState::IDLE_LEFTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = OguAniState::IDLE_LEFT;
			}

		}
		else if (m_CurMS == OguAniState::WALK_LEFTDOWN || m_CurMS == OguAniState::RUN_LEFTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = OguAniState::IDLE_LEFTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = OguAniState::IDLE_LEFT;
			}
		}
		else if (m_CurMS == OguAniState::WALK_LEFT || m_CurMS == OguAniState::RUN_LEFT)
		{
			m_CurMS = OguAniState::IDLE_LEFT;
		}
	}

	if (KEY_RELEASED(KEY::RIGHT))
	{
		if (m_CurMS == OguAniState::WALK_RIGHTUP || m_CurMS == OguAniState::RUN_RIGHTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = OguAniState::IDLE_RIGHTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = OguAniState::IDLE_RIGHT;
			}
		}
		else if (m_CurMS == OguAniState::WALK_RIGHTDOWN || m_CurMS == OguAniState::RUN_RIGHTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = OguAniState::IDLE_RIGHTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = OguAniState::IDLE_RIGHT;
			}
		}
		else if (m_CurMS == OguAniState::WALK_RIGHT || m_CurMS == OguAniState::RUN_RIGHT)
		{
			m_CurMS = OguAniState::IDLE_RIGHT;
		}
	}

	if (KEY_RELEASED(KEY::UP))
	{
		if (m_CurMS == OguAniState::WALK_LEFTUP || m_CurMS == OguAniState::RUN_LEFTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = OguAniState::IDLE_LEFTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = OguAniState::IDLE_BACK;
			}
		}
		else if (m_CurMS == OguAniState::WALK_RIGHTUP || m_CurMS == OguAniState::RUN_RIGHTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = OguAniState::IDLE_RIGHTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = OguAniState::IDLE_RIGHT;
			}
		}
		else if (m_CurMS == OguAniState::WALK_UP || m_CurMS == OguAniState::RUN_UP)
		{
			m_CurMS = OguAniState::IDLE_BACK;
		}
	}

	if (KEY_RELEASED(KEY::DOWN))
	{
		if (m_CurMS == OguAniState::WALK_LEFTDOWN || m_CurMS == OguAniState::RUN_LEFTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = OguAniState::IDLE_LEFTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = OguAniState::IDLE;
			}
		}
		else if (m_CurMS == OguAniState::WALK_RIGHTDOWN || m_CurMS == OguAniState::RUN_RIGHTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = OguAniState::IDLE_RIGHTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = OguAniState::IDLE;
			}
		}
		else if (m_CurMS == OguAniState::WALK_DOWN || m_CurMS == OguAniState::RUN_DOWN)
		{
			m_CurMS = OguAniState::IDLE;
		}
	}

	// Move
	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::UP))
	{
		vPos.y += DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= DT * m_Speed;
	}

	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::AniState()
{
	// Animation
	switch (m_CurMS)
	{
	case OguAniState::IDLE:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE, 10, true);
		break;
	case OguAniState::IDLE_BACK:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_BACK, 10, true);
		break;
	case OguAniState::IDLE_LEFT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_LEFT, 10, true);
		break;
	case OguAniState::IDLE_RIGHT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_RIGHT, 10, true);
		break;
	case OguAniState::IDLE_LEFTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_LEFTDOWN, 10, true);
		break;
	case OguAniState::IDLE_LEFTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_LEFTUP, 10, true);
		break;
	case OguAniState::IDLE_RIGHTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_RIGHTDOWN, 10, true);
		break;
	case OguAniState::IDLE_RIGHTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_RIGHTUP, 10, true);
		break;
	case OguAniState::WALK_DOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_DOWN, 10, true);
		break;
	case OguAniState::WALK_UP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_UP, 10, true);
		break;
	case OguAniState::WALK_LEFT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_LEFT, 10, true);
		break;
	case OguAniState::WALK_RIGHT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_RIGHT, 10, true);
		break;
	case OguAniState::WALK_LEFTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_LEFTDOWN, 10, true);
		break;
	case OguAniState::WALK_LEFTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_LEFTUP, 10, true);
		break;
	case OguAniState::WALK_RIGHTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_RIGHTDOWN, 10, true);
		break;
	case OguAniState::WALK_RIGHTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_RIGHTUP, 10, true);
		break;
	case OguAniState::RUN_DOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_DOWN, 10, true);
		break;
	case OguAniState::RUN_UP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_UP, 10, true);
		break;
	case OguAniState::RUN_LEFT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_LEFT, 10, true);
		break;
	case OguAniState::RUN_RIGHT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_RIGHT, 10, true);
		break;
	case OguAniState::RUN_LEFTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_LEFTDOWN, 10, true);
		break;
	case OguAniState::RUN_LEFTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_LEFTUP, 10, true);
		break;
	case OguAniState::RUN_RIGHTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_RIGHTDOWN, 10, true);
		break;
	case OguAniState::RUN_RIGHTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_RIGHTUP, 10, true);
		break;
	case OguAniState::IDLE_DANCE:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_DANCE, 10, true);
		break;
	case OguAniState::DANCE:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_DANCE, 10, true);
		break;
	case OguAniState::ROLL:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_ROLL, 10, true);
		break;
	case OguAniState::SWING_DOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_SWING_DOWN, 10, false);
	default:
		break;
	}
}

void CPlayerScript::RunParticle()
{
	// 달리는 파티클 소환
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"RunParticle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"particle\\ogu_run.particle";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	pParticleObj->ParticleSystem()->LoadFromFile(File);
	fclose(File);

	// 삭제되는 시간을 지정해준다.
	pParticleObj->AddComponent(new CCountDownDeleteScript);
	CScript* pScript = pParticleObj->GetScript("CCountDownDeleteScript");
	CCountDownDeleteScript* pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
	pCountDown->SetSaveTime(TIME);
	pCountDown->SetDeadTime(4.f);

	Vec3 spawnPos = Vec3(0.f, 0.f, 0.f);

	switch (m_CurMS)
	{
	case OguAniState::RUN_DOWN:
		spawnPos = Vec3(0.f, 30.f, 0.f);
		break;
	case OguAniState::RUN_UP:
		spawnPos = Vec3(0.f, -30.f, 0.f);
		break;
	case OguAniState::RUN_LEFT:
		spawnPos = Vec3(-60.f, 0.f, 0.f);
		break;
	case OguAniState::RUN_RIGHT:
		spawnPos = Vec3(60.f, 0.f, 0.f);
		break;
	case OguAniState::RUN_LEFTDOWN:
		spawnPos = Vec3(-30.f, 30.f, 0.f);
		break;
	case OguAniState::RUN_LEFTUP:
		spawnPos = Vec3(-30.f, -30.f, 0.f);
		break;
	case OguAniState::RUN_RIGHTDOWN:
		spawnPos = Vec3(30.f, 30.f, 0.f);
		break;
	case OguAniState::RUN_RIGHTUP:
		spawnPos = Vec3(30.f, -30.f, 0.f);
		break;
	default:
		break;
	}

	pParticleObj->Transform()->SetRelativePos(Transform()->GetRelativePos() + spawnPos);

	CreateObject(pParticleObj, 0);
}

void CPlayerScript::Dance()
{
	m_CurMS = OguAniState::DANCE;

	// 포인트 라이트 추가
	// 범위가 점점 작아짐
	CGameObject* pPointLight = new CGameObject;
	pPointLight->SetName(L"OguDnacePointLight");
	pPointLight->AddComponent(new CTransform);
	pPointLight->AddComponent(new CLight2D);

	pPointLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pPointLight->Light2D()->SetLightColor(Vec3(0.2f, 1.f, 0.2f));
	pPointLight->Light2D()->SetRadius(80.f);
	pPointLight->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());

	// 삭제되는 시간을 지정해준다.
	pPointLight->AddComponent(new CCountDownDeleteScript);
	CScript* pScript = pPointLight->GetScript("CCountDownDeleteScript");
	CCountDownDeleteScript* pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
	pCountDown->SetSaveTime(TIME);
	pCountDown->SetDeadTime(4.f);

	pPointLight->AddComponent(new COguDancePointLightScript);

	//GetOwner()->AddChild(pPointLight);
	CreateObject(pPointLight, 0);

	CGameObject* pPointLight2 = new CGameObject;
	pPointLight2->SetName(L"OguDnacePointLight2");
	pPointLight2->AddComponent(new CTransform);
	pPointLight2->AddComponent(new CLight2D);

	pPointLight2->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pPointLight2->Light2D()->SetLightColor(Vec3(0.8667f, 1.f, 0.8392f));
	pPointLight2->Light2D()->SetRadius(80.f);
	pPointLight2->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());

	// 삭제되는 시간을 지정해준다.
	pPointLight2->AddComponent(new CCountDownDeleteScript);
	pScript = pPointLight2->GetScript("CCountDownDeleteScript");
	pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
	pCountDown->SetSaveTime(TIME);
	pCountDown->SetDeadTime(4.f);

	pPointLight2->AddComponent(new COguDancePointLightScript);
	pScript = pPointLight2->GetScript("COguDancePointLightScript");
	COguDancePointLightScript* pDPLS = dynamic_cast<COguDancePointLightScript*>(pScript);
	pDPLS->SetMaxRadius(50.f);
	pDPLS->SetMinRadius(30.f);

	//GetOwner()->AddChild(pPointLight);
	CreateObject(pPointLight2, 0);

	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"DanceParticle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"particle\\ogu_dance.particle";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	pParticleObj->ParticleSystem()->LoadFromFile(File);
	fclose(File);

	// 삭제되는 시간을 지정해준다.
	pParticleObj->AddComponent(new CCountDownDeleteScript);
	pScript = pParticleObj->GetScript("CCountDownDeleteScript");
	pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
	pCountDown->SetSaveTime(TIME);
	pCountDown->SetDeadTime(4.f);

	pParticleObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());

	CreateObject(pParticleObj, 0);
}
