#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/enum.h>

#include "CCountDownDeleteScript.h"
#include "COguDancePointLightScript.h"

CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_CurPS(PlayerState::NONE)
	, m_StartFrmIdx(0)
	, m_Speed(200.f)
	, m_MinSpeed(200.f)
	, m_MaxSpeed(300.f)
	, m_IsRun(false)
	, m_MoveCount(0)
	, m_SaveFinalActionTime(0)
	, m_SaveFinalDiagonalTime(0)
	, m_AllowedTime(0.1f)
	, m_CurAS(OguAniState::IDLE)
	, m_IdleDanceTime(8.f)
	, m_IsRunParticle(false)
	, m_PreAS(OguAniState::IDLE)
	, m_SaveDanceTime(0.f)
	, m_DanceTime(1.5f)
	, m_IsDance(false)
	, m_RollingSpeedMax(400.f)
	, m_RollingSpeed(400.f)
	, m_RollingDeceleration(5.f)
	, m_SaveRollingTime(0.f)
	, m_RollingDelay(2.f)
	, m_Interaction(false)
{	
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Test", &m_Texture);
	//AddScriptParam(SCRIPT_PARAM::BOOL1, "Swing", &m_IsSwing);
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
	if (m_CurPS == PlayerState::LIFT_START && !m_Interaction)
	{
		m_StartFrmIdx = 0;
		LiftMove();
	}

	if (m_CurPS == PlayerState::NONE)
	{
		m_StartFrmIdx = 0;
		Move();

		// Q 버튼을 누르면 춤을 춘다.
		if (KEY_TAP(KEY::Q) && !m_IsDance)
		{
			m_SaveDanceTime = TIME;
			m_IsDance = true;
			DanceEffect();
		}
		if (KEY_PRESSED(KEY::Q))
		{
			if (TIME - m_SaveDanceTime >= m_DanceTime)
			{
				m_CurAS = OguAniState::DANCE;
			}
		}
		if (KEY_RELEASED(KEY::Q))
		{
			DanceEffectDelete();
			m_IsDance = false;
		}

		if (TIME - m_SaveFinalActionTime >= m_IdleDanceTime)
		{
			m_CurAS = OguAniState::IDLE_DANCE;
		}

		// A 버튼을 누르면 채를 휘두른다.
		if (KEY_TAP(KEY::A))
		{
			Swing();
		}
	}

	// D 버튼을 누르면 구른다.
	if (KEY_TAP(KEY::D) && m_CurPS == PlayerState::NONE)
	{
		// 구르고 난 후 딜레이 시간이 있다.
		if (TIME - m_SaveRollingTime >= m_RollingDelay)
		{
			RollingStart();
		}
		else
		{
			// 아직 사용할 수 없다는 문구 출력해야 함
		}
	}		
	if (m_CurPS == PlayerState::ROLLING)
	{
		Rolling();
	}

	AniFinishCheck();
	
	AniState();
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 자신의 자식이면 return
	if (_OtherObject->GetParent() == GetOwner())
	{
		return;
	}

	// 물리적으로 충돌되게 하기
	Vec3 force = Vec3(0.f, 0.f, 0.f);
	float speed = m_Speed;
	float rollSpeed = m_RollingSpeed;

	switch (m_CurAS)
	{
	case OguAniState::WALK_DOWN:
	case OguAniState::RUN_DOWN:
	case OguAniState::LIFT_WALK_DOWN:
		force = Vec3(0.f, -speed, 0.f);
		break;
	case OguAniState::WALK_UP:
	case OguAniState::RUN_UP:
	case OguAniState::LIFT_WALK_UP:
		force = Vec3(0.f, speed, 0.f);
		break;
	case OguAniState::WALK_LEFT:
	case OguAniState::RUN_LEFT:
	case OguAniState::LIFT_WALK_LEFT:
		force = Vec3(-speed, 0.f, 0.f);
		break;
	case OguAniState::WALK_RIGHT:
	case OguAniState::RUN_RIGHT:
	case OguAniState::LIFT_WALK_RIGHT:
		force = Vec3(speed, 0.f, 0.f);
		break;
	case OguAniState::WALK_LEFTDOWN:
	case OguAniState::RUN_LEFTDOWN:
	case OguAniState::LIFT_WALK_LEFTDOWN:
		force = Vec3(-speed, -speed, 0.f);
		break;
	case OguAniState::WALK_LEFTUP:
	case OguAniState::RUN_LEFTUP:
	case OguAniState::LIFT_WALK_LEFTUP:
		force = Vec3(-speed, speed, 0.f);
		break;
	case OguAniState::WALK_RIGHTDOWN:
	case OguAniState::RUN_RIGHTDOWN:
	case OguAniState::LIFT_WALK_RIGHTDOWN:
		force = Vec3(speed, -speed, 0.f);
		break;
	case OguAniState::WALK_RIGHTUP:
	case OguAniState::RUN_RIGHTUP:
	case OguAniState::LIFT_WALK_RIGHTUP:
		force = Vec3(speed, speed, 0.f);
		break;
	case OguAniState::ROLL_DOWN:
		force = Vec3(0.f, -rollSpeed, 0.f);
		break;
	case OguAniState::ROLL_UP:
		force = Vec3(0.f, rollSpeed, 0.f);
		break;
	case OguAniState::ROLL_LEFT:
		force = Vec3(-rollSpeed, 0.f, 0.f);
		break;
	case OguAniState::ROLL_RIGHT:
		force = Vec3(rollSpeed, 0.f, 0.f);
		break;
	case OguAniState::ROLL_LEFTDOWN:
		force = Vec3(-rollSpeed, -rollSpeed, 0.f);
		break;
	case OguAniState::ROLL_LEFTUP:
		force = Vec3(-rollSpeed, rollSpeed, 0.f);
		break;
	case OguAniState::ROLL_RIGHTDOWN:
		force = Vec3(rollSpeed, -rollSpeed, 0.f);
		break;
	case OguAniState::ROLL_RIGHTUP:
		force = Vec3(rollSpeed, rollSpeed, 0.f);
		break;
	default:
		break;
	}

	GetOwner()->RigidBody()->SetForce(force);
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
		m_SaveFinalActionTime = TIME;
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
			m_CurAS = OguAniState::RUN_LEFTUP;
		}
		else
		{
			m_CurAS = OguAniState::WALK_LEFTUP;
		}

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::DOWN))
	{
		if (m_IsRun)
		{
			m_CurAS = OguAniState::RUN_LEFTDOWN;
		}
		else
		{
			m_CurAS = OguAniState::WALK_LEFTDOWN;
		}

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::UP))
	{
		if (m_IsRun)
		{
			m_CurAS = OguAniState::RUN_RIGHTUP;
		}
		else
		{
			m_CurAS = OguAniState::WALK_RIGHTUP;
		}

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::DOWN))
	{
		if (m_IsRun)
		{
			m_CurAS = OguAniState::RUN_RIGHTDOWN;
		}
		else
		{
			m_CurAS = OguAniState::WALK_RIGHTDOWN;
		}

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (TIME - m_SaveFinalDiagonalTime > m_AllowedTime)
	{
		if (KEY_PRESSED(KEY::LEFT))
		{
			if (m_IsRun)
			{
				m_CurAS = OguAniState::RUN_LEFT;
			}
			else
			{
				m_CurAS = OguAniState::WALK_LEFT;
			}
		}
		else if (KEY_PRESSED(KEY::RIGHT))
		{
			if (m_IsRun)
			{
				m_CurAS = OguAniState::RUN_RIGHT;
			}
			else
			{
				m_CurAS = OguAniState::WALK_RIGHT;
			}
		}
		else if (KEY_PRESSED(KEY::UP))
		{
			if (m_IsRun)
			{
				m_CurAS = OguAniState::RUN_UP;
			}
			else
			{
				m_CurAS = OguAniState::WALK_UP;
			}
		}
		else if (KEY_PRESSED(KEY::DOWN))
		{
			if (m_IsRun)
			{
				m_CurAS = OguAniState::RUN_DOWN;
			}
			else
			{
				m_CurAS = OguAniState::WALK_DOWN;
			}
		}

	}

	if (KEY_RELEASED(KEY::LEFT))
	{
		if (m_CurAS == OguAniState::WALK_LEFTUP || m_CurAS == OguAniState::RUN_LEFTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::IDLE_LEFTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::IDLE_LEFT;
			}

		}
		else if (m_CurAS == OguAniState::WALK_LEFTDOWN || m_CurAS == OguAniState::RUN_LEFTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::IDLE_LEFTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::IDLE_LEFT;
			}
		}
		else if (m_CurAS == OguAniState::WALK_LEFT || m_CurAS == OguAniState::RUN_LEFT)
		{
			m_CurAS = OguAniState::IDLE_LEFT;
		}
	}

	if (KEY_RELEASED(KEY::RIGHT))
	{
		if (m_CurAS == OguAniState::WALK_RIGHTUP || m_CurAS == OguAniState::RUN_RIGHTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::IDLE_RIGHTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::IDLE_RIGHT;
			}
		}
		else if (m_CurAS == OguAniState::WALK_RIGHTDOWN || m_CurAS == OguAniState::RUN_RIGHTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::IDLE_RIGHTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::IDLE_RIGHT;
			}
		}
		else if (m_CurAS == OguAniState::WALK_RIGHT || m_CurAS == OguAniState::RUN_RIGHT)
		{
			m_CurAS = OguAniState::IDLE_RIGHT;
		}
	}

	if (KEY_RELEASED(KEY::UP))
	{
		if (m_CurAS == OguAniState::WALK_LEFTUP || m_CurAS == OguAniState::RUN_LEFTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::IDLE_LEFTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::IDLE_BACK;
			}
		}
		else if (m_CurAS == OguAniState::WALK_RIGHTUP || m_CurAS == OguAniState::RUN_RIGHTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::IDLE_RIGHTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::IDLE_RIGHT;
			}
		}
		else if (m_CurAS == OguAniState::WALK_UP || m_CurAS == OguAniState::RUN_UP)
		{
			m_CurAS = OguAniState::IDLE_BACK;
		}
	}

	if (KEY_RELEASED(KEY::DOWN))
	{
		if (m_CurAS == OguAniState::WALK_LEFTDOWN || m_CurAS == OguAniState::RUN_LEFTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::IDLE_LEFTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::IDLE;
			}
		}
		else if (m_CurAS == OguAniState::WALK_RIGHTDOWN || m_CurAS == OguAniState::RUN_RIGHTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::IDLE_RIGHTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::IDLE;
			}
		}
		else if (m_CurAS == OguAniState::WALK_DOWN || m_CurAS == OguAniState::RUN_DOWN)
		{
			m_CurAS = OguAniState::IDLE;
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
	switch (m_CurAS)
	{
	case OguAniState::IDLE:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE, 10, true);
		break;
	case OguAniState::IDLE_BACK:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE_BACK, 10, true);
		break;
	case OguAniState::IDLE_LEFT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE_LEFT, 10, true);
		break;
	case OguAniState::IDLE_RIGHT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE_RIGHT, 10, true);
		break;
	case OguAniState::IDLE_LEFTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE_LEFTDOWN, 10, true);
		break;
	case OguAniState::IDLE_LEFTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE_LEFTUP, 10, true);
		break;
	case OguAniState::IDLE_RIGHTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE_RIGHTDOWN, 10, true);
		break;
	case OguAniState::IDLE_RIGHTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE_RIGHTUP, 10, true);
		break;
	case OguAniState::WALK_DOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_WALK_DOWN, 10, true);
		break;
	case OguAniState::WALK_UP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_WALK_UP, 10, true);
		break;
	case OguAniState::WALK_LEFT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_WALK_LEFT, 10, true);
		break;
	case OguAniState::WALK_RIGHT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_WALK_RIGHT, 10, true);
		break;
	case OguAniState::WALK_LEFTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_WALK_LEFTDOWN, 10, true);
		break;
	case OguAniState::WALK_LEFTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_WALK_LEFTUP, 10, true);
		break;
	case OguAniState::WALK_RIGHTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_WALK_RIGHTDOWN, 10, true);
		break;
	case OguAniState::WALK_RIGHTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_WALK_RIGHTUP, 10, true);
		break;
	case OguAniState::RUN_DOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_RUN_DOWN, 10, true);
		break;
	case OguAniState::RUN_UP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_RUN_UP, 10, true);
		break;
	case OguAniState::RUN_LEFT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_RUN_LEFT, 10, true);
		break;
	case OguAniState::RUN_RIGHT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_RUN_RIGHT, 10, true);
		break;
	case OguAniState::RUN_LEFTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_RUN_LEFTDOWN, 10, true);
		break;
	case OguAniState::RUN_LEFTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_RUN_LEFTUP, 10, true);
		break;
	case OguAniState::RUN_RIGHTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_RUN_RIGHTDOWN, 10, true);
		break;
	case OguAniState::RUN_RIGHTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_RUN_RIGHTUP, 10, true);
		break;
	case OguAniState::IDLE_DANCE:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE_DANCE, 10, true);
		break;
	case OguAniState::DANCE:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_DANCE, 10, true);
		break;
	case OguAniState::ROLL_DOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_ROLL_DOWN, 15, false);
		break;
	case OguAniState::ROLL_UP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_ROLL_UP, 15, false);
		break;
	case OguAniState::ROLL_LEFT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_ROLL_LEFT, 15, false);
		break;
	case OguAniState::ROLL_RIGHT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_ROLL_RIGHT, 15, false);
		break;
	case OguAniState::ROLL_LEFTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_ROLL_LEFTDOWN, 15, false);
		break;
	case OguAniState::ROLL_LEFTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_ROLL_LEFTUP, 15, false);
		break;
	case OguAniState::ROLL_RIGHTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_ROLL_RIGHTDOWN, 15, false);
		break;
	case OguAniState::ROLL_RIGHTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_ROLL_RIGHTUP, 15, false);
		break;
	case OguAniState::SWING_DOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_SWING_DOWN, 10, false);
		break;
	case OguAniState::SWING_UP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_SWING_UP, 10, false);
		break;
	case OguAniState::SWING_LEFT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_SWING_LEFT, 10, false);
		break;
	case OguAniState::SWING_RIGHT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_SWING_RIGHT, 10, false);
		break;
	case OguAniState::SWING_LEFTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_SWING_LEFTDOWN, 10, false);
		break;
	case OguAniState::SWING_LEFTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_SWING_LEFTUP, 10, false);
		break;
	case OguAniState::SWING_RIGHTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_SWING_RIGHTDOWN, 10, false);
		break;
	case OguAniState::SWING_RIGHTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_SWING_RIGHTUP, 10, false);
		break;
	case OguAniState::LIFT_IDLE1_DOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_DOWN, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE1_UP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_UP, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE1_LEFT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_LEFT, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE1_RIGHT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_RIGHT, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE1_LEFTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_LEFTDOWN, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE1_LEFTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_LEFTUP, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE1_RIGHTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_RIGHTDOWN, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE1_RIGHTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_RIGHTUP, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE2_DOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_DOWN, 10, false);
		break;
	case OguAniState::LIFT_IDLE2_UP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_UP, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE2_LEFT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_LEFT, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE2_RIGHT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_RIGHT, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE2_LEFTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_LEFTDOWN, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE2_LEFTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_LEFTUP, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE2_RIGHTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_RIGHTDOWN, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_IDLE2_RIGHTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_RIGHTUP, 10, false, m_StartFrmIdx);
		break;
	case OguAniState::LIFT_WALK_DOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_DOWN, 10, true);
		break;
	case OguAniState::LIFT_WALK_UP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_UP, 10, true);
		break;
	case OguAniState::LIFT_WALK_LEFT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_LEFT, 10, true);
		break;
	case OguAniState::LIFT_WALK_RIGHT:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_RIGHT, 10, true);
		break;
	case OguAniState::LIFT_WALK_LEFTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_LEFTDOWN, 10, true);
		break;
	case OguAniState::LIFT_WALK_LEFTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_LEFTUP, 10, true);
		break;
	case OguAniState::LIFT_WALK_RIGHTDOWN:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_RIGHTDOWN, 10, true);
		break;
	case OguAniState::LIFT_WALK_RIGHTUP:
		FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_RIGHTUP, 10, true);
		break;
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

	switch (m_CurAS)
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

void CPlayerScript::DanceEffect()
{
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
	pCountDown->SetDeadTime(1.5f);

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
	pCountDown->SetDeadTime(1.5f);

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
	pCountDown->SetDeadTime(1.5f);

	pParticleObj->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());

	CreateObject(pParticleObj, 0);

	m_vDanceEffects.push_back(pPointLight);
	m_vDanceEffects.push_back(pPointLight2);
	m_vDanceEffects.push_back(pParticleObj);
}

void CPlayerScript::DanceEffectDelete()
{
	for (auto i : m_vDanceEffects)
	{
		DeleteObject(i);
	}
	m_vDanceEffects.clear();
}

void CPlayerScript::Swing()
{
	if (m_CurPS == PlayerState::NONE)
	{
		switch (m_CurAS)
		{
		case OguAniState::DANCE:
		case OguAniState::IDLE_DANCE:
		case OguAniState::IDLE:
		case OguAniState::WALK_DOWN:
		case OguAniState::RUN_DOWN:
			m_PreAS = OguAniState::IDLE;
			m_CurAS = OguAniState::SWING_DOWN;
			break;
		case OguAniState::IDLE_BACK:
		case OguAniState::WALK_UP:
		case OguAniState::RUN_UP:
			m_PreAS = OguAniState::IDLE_BACK;
			m_CurAS = OguAniState::SWING_UP;
			break;
		case OguAniState::IDLE_LEFT:
		case OguAniState::WALK_LEFT:
		case OguAniState::RUN_LEFT:
			m_PreAS = OguAniState::IDLE_LEFT;
			m_CurAS = OguAniState::SWING_LEFT;
			break;
		case OguAniState::IDLE_RIGHT:
		case OguAniState::WALK_RIGHT:
		case OguAniState::RUN_RIGHT:
			m_PreAS = OguAniState::IDLE_RIGHT;
			m_CurAS = OguAniState::SWING_RIGHT;
			break;
		case OguAniState::IDLE_LEFTDOWN:
		case OguAniState::WALK_LEFTDOWN:
		case OguAniState::RUN_LEFTDOWN:
			m_PreAS = OguAniState::IDLE_LEFTDOWN;
			m_CurAS = OguAniState::SWING_LEFTDOWN;
			break;
		case OguAniState::IDLE_LEFTUP:
		case OguAniState::WALK_LEFTUP:
		case OguAniState::RUN_LEFTUP:
			m_PreAS = OguAniState::IDLE_LEFTUP;
			m_CurAS = OguAniState::SWING_LEFTUP;
			break;
		case OguAniState::IDLE_RIGHTDOWN:
		case OguAniState::WALK_RIGHTDOWN:
		case OguAniState::RUN_RIGHTDOWN:
			m_PreAS = OguAniState::IDLE_RIGHTDOWN;
			m_CurAS = OguAniState::SWING_RIGHTDOWN;
			break;
		case OguAniState::IDLE_RIGHTUP:
		case OguAniState::WALK_RIGHTUP:
		case OguAniState::RUN_RIGHTUP:
			m_PreAS = OguAniState::IDLE_RIGHTUP;
			m_CurAS = OguAniState::SWING_RIGHTUP;
			break;
		default:
			break;
		}
	}
	
	m_CurPS = PlayerState::SWING;
}

void CPlayerScript::AniFinishCheck()
{
	if (m_CurPS == PlayerState::SWING)
	{
		if (FlipBookComponent()->GetIsFinish())
		{
			m_CurAS = m_PreAS;
			m_CurPS = PlayerState::NONE;
		}
	}

	if (m_CurPS == PlayerState::ROLLING)
	{
		if (FlipBookComponent()->GetIsFinish())
		{
			m_CurAS = m_PreAS;
			m_CurPS = PlayerState::NONE;
			m_SaveRollingTime = TIME;
		}
	}

	if (m_CurPS == PlayerState::LIFT_START)
	{
		if (FlipBookComponent()->GetIsFinish())
		{
			if (m_Interaction)
			{
				m_Interaction = false;
			}
		}
	}

	if (m_CurPS == PlayerState::LIFT_END)
	{
		if (FlipBookComponent()->GetIsFinish())
		{
			if (m_Interaction)
			{
				m_CurPS = PlayerState::NONE;
				m_Interaction = false;

				switch (m_CurAS)
				{
				case OguAniState::LIFT_IDLE2_DOWN:
					m_CurAS = OguAniState::IDLE;
					break;
				case OguAniState::LIFT_IDLE2_UP:
					m_CurAS = OguAniState::IDLE_BACK;
					break;
				case OguAniState::LIFT_IDLE2_LEFT:
					m_CurAS = OguAniState::IDLE_LEFT;
					break;
				case OguAniState::LIFT_IDLE2_RIGHT:
					m_CurAS = OguAniState::IDLE_RIGHT;
					break;
				case OguAniState::LIFT_IDLE2_LEFTDOWN:
					m_CurAS = OguAniState::IDLE_LEFTDOWN;
					break;
				case OguAniState::LIFT_IDLE2_LEFTUP:
					m_CurAS = OguAniState::IDLE_LEFTUP;
					break;
				case OguAniState::LIFT_IDLE2_RIGHTDOWN:
					m_CurAS = OguAniState::IDLE_RIGHTDOWN;
					break;
				case OguAniState::LIFT_IDLE2_RIGHTUP:
					m_CurAS = OguAniState::IDLE_RIGHTUP;
					break;
				default:
					break;
				}
			}
		}
	}
	
}

void CPlayerScript::RollingStart()
{
	if (m_CurPS == PlayerState::NONE)
	{
		switch (m_CurAS)
		{
		case OguAniState::DANCE:
		case OguAniState::IDLE_DANCE:
		case OguAniState::IDLE:
		case OguAniState::WALK_DOWN:
		case OguAniState::RUN_DOWN:
			m_PreAS = OguAniState::IDLE;
			m_CurAS = OguAniState::ROLL_DOWN;
			break;
		case OguAniState::IDLE_BACK:
		case OguAniState::WALK_UP:
		case OguAniState::RUN_UP:
			m_PreAS = OguAniState::IDLE_BACK;
			m_CurAS = OguAniState::ROLL_UP;
			break;
		case OguAniState::IDLE_LEFT:
		case OguAniState::WALK_LEFT:
		case OguAniState::RUN_LEFT:
			m_PreAS = OguAniState::IDLE_LEFT;
			m_CurAS = OguAniState::ROLL_LEFT;
			break;
		case OguAniState::IDLE_RIGHT:
		case OguAniState::WALK_RIGHT:
		case OguAniState::RUN_RIGHT:
			m_PreAS = OguAniState::IDLE_RIGHT;
			m_CurAS = OguAniState::ROLL_RIGHT;
			break;
		case OguAniState::IDLE_LEFTDOWN:
		case OguAniState::WALK_LEFTDOWN:
		case OguAniState::RUN_LEFTDOWN:
			m_PreAS = OguAniState::IDLE_LEFTDOWN;
			m_CurAS = OguAniState::ROLL_LEFTDOWN;
			break;
		case OguAniState::IDLE_LEFTUP:
		case OguAniState::WALK_LEFTUP:
		case OguAniState::RUN_LEFTUP:
			m_PreAS = OguAniState::IDLE_LEFTUP;
			m_CurAS = OguAniState::ROLL_LEFTUP;
			break;
		case OguAniState::IDLE_RIGHTDOWN:
		case OguAniState::WALK_RIGHTDOWN:
		case OguAniState::RUN_RIGHTDOWN:
			m_PreAS = OguAniState::IDLE_RIGHTDOWN;
			m_CurAS = OguAniState::ROLL_RIGHTDOWN;
			break;
		case OguAniState::IDLE_RIGHTUP:
		case OguAniState::WALK_RIGHTUP:
		case OguAniState::RUN_RIGHTUP:
			m_PreAS = OguAniState::IDLE_RIGHTUP;
			m_CurAS = OguAniState::ROLL_RIGHTUP;
			break;
		default:
			break;
		}
	}

	m_CurPS = PlayerState::ROLLING;
	m_SaveRollingTime = TIME;

	RollingParticle();
}

void CPlayerScript::Rolling()
{
	// 무적상태가 되면서 지금 상태에 해당하는 방향을 향해 앞으로 나아가며 구른다.
	// 순간적으로 앞으로 나가고, 감속한다.
	Vec3 vPos = Transform()->GetRelativePos();

	if (TIME - m_SaveRollingTime >= 0.3f)
	{
		m_RollingSpeed -= m_RollingDeceleration;
	}
	else
	{
		m_RollingSpeed = m_RollingSpeedMax;
	}

	switch (m_CurAS)
	{
	case OguAniState::ROLL_DOWN:
		vPos.y -= DT * m_RollingSpeed;
		break;
	case OguAniState::ROLL_UP:
		vPos.y += DT * m_RollingSpeed;
		break;
	case OguAniState::ROLL_LEFT:
		vPos.x -= DT * m_RollingSpeed;
		break;
	case OguAniState::ROLL_RIGHT:
		vPos.x += DT * m_RollingSpeed;
		break;
	case OguAniState::ROLL_LEFTDOWN:
		vPos.x -= DT * m_RollingSpeed;
		vPos.y -= DT * m_RollingSpeed;
		break;
	case OguAniState::ROLL_LEFTUP:
		vPos.x -= DT * m_RollingSpeed;
		vPos.y += DT * m_RollingSpeed;
		break;
	case OguAniState::ROLL_RIGHTDOWN:
		vPos.x += DT * m_RollingSpeed;
		vPos.y -= DT * m_RollingSpeed;
		break;
	case OguAniState::ROLL_RIGHTUP:
		vPos.x += DT * m_RollingSpeed;
		vPos.y += DT * m_RollingSpeed;
		break;
	default:
		break;
	}

	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::RollingParticle()
{
	// 파티클 소환
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"RollingParticle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"particle\\ogu_rolling.particle";

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

	switch (m_CurAS)
	{
	case OguAniState::ROLL_DOWN:
		spawnPos = Vec3(0.f, 30.f, 0.f);
		break;
	case OguAniState::ROLL_UP:
		spawnPos = Vec3(0.f, -30.f, 0.f);
		break;
	case OguAniState::ROLL_LEFT:
		spawnPos = Vec3(60.f, 0.f, 0.f);
		break;
	case OguAniState::ROLL_RIGHT:
		spawnPos = Vec3(-60.f, 0.f, 0.f);
		break;
	case OguAniState::ROLL_LEFTDOWN:
		spawnPos = Vec3(-30.f, 30.f, 0.f);
		break;
	case OguAniState::ROLL_LEFTUP:
		spawnPos = Vec3(-30.f, -30.f, 0.f);
		break;
	case OguAniState::ROLL_RIGHTDOWN:
		spawnPos = Vec3(30.f, 30.f, 0.f);
		break;
	case OguAniState::ROLL_RIGHTUP:
		spawnPos = Vec3(30.f, -30.f, 0.f);
		break;
	default:
		break;
	}

	pParticleObj->Transform()->SetRelativePos(Transform()->GetRelativePos() + spawnPos);

	CreateObject(pParticleObj, 0);
}

void CPlayerScript::LiftStart()
{
	m_CurPS = PlayerState::LIFT_START;
	m_Interaction = true;

	switch (m_CurAS)
	{
	case OguAniState::DANCE:
	case OguAniState::IDLE_DANCE:
	case OguAniState::IDLE:
	case OguAniState::WALK_DOWN:
	case OguAniState::RUN_DOWN:
		m_CurAS = OguAniState::LIFT_IDLE1_DOWN;
		break;
	case OguAniState::IDLE_BACK:
	case OguAniState::WALK_UP:
	case OguAniState::RUN_UP:
		m_CurAS = OguAniState::LIFT_IDLE1_UP;
		break;
	case OguAniState::IDLE_LEFT:
	case OguAniState::WALK_LEFT:
	case OguAniState::RUN_LEFT:
		m_CurAS = OguAniState::LIFT_IDLE1_LEFT;
		break;
	case OguAniState::IDLE_RIGHT:
	case OguAniState::WALK_RIGHT:
	case OguAniState::RUN_RIGHT:
		m_CurAS = OguAniState::LIFT_IDLE1_RIGHT;
		break;
	case OguAniState::IDLE_LEFTDOWN:
	case OguAniState::WALK_LEFTDOWN:
	case OguAniState::RUN_LEFTDOWN:
		m_CurAS = OguAniState::LIFT_IDLE1_LEFTDOWN;
		break;
	case OguAniState::IDLE_LEFTUP:
	case OguAniState::WALK_LEFTUP:
	case OguAniState::RUN_LEFTUP:
		m_CurAS = OguAniState::LIFT_IDLE1_LEFTUP;
		break;
	case OguAniState::IDLE_RIGHTDOWN:
	case OguAniState::WALK_RIGHTDOWN:
	case OguAniState::RUN_RIGHTDOWN:
		m_CurAS = OguAniState::LIFT_IDLE1_RIGHTDOWN;
		break;
	case OguAniState::IDLE_RIGHTUP:
	case OguAniState::WALK_RIGHTUP:
	case OguAniState::RUN_RIGHTUP:
		m_CurAS = OguAniState::LIFT_IDLE1_RIGHTUP;
		break;
	default:
		break;
	}
}

void CPlayerScript::LiftMove()
{
	Vec3 vPos = Transform()->GetRelativePos();

	// 마지막으로 움직인 시간
	if (KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT) || KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::DOWN))
	{
		m_SaveFinalActionTime = TIME;
	}

	if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::UP))
	{
		m_CurAS = OguAniState::LIFT_WALK_LEFTUP;

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::DOWN))
	{
		m_CurAS = OguAniState::LIFT_WALK_LEFTDOWN;

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::UP))
	{
		m_CurAS = OguAniState::LIFT_WALK_RIGHTUP;

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::DOWN))
	{
		m_CurAS = OguAniState::LIFT_WALK_RIGHTDOWN;

		m_SaveFinalDiagonalTime = TIME;
	}
	else if (TIME - m_SaveFinalDiagonalTime > m_AllowedTime)
	{
		if (KEY_PRESSED(KEY::LEFT))
		{
			m_CurAS = OguAniState::LIFT_WALK_LEFT;
		}
		else if (KEY_PRESSED(KEY::RIGHT))
		{
			m_CurAS = OguAniState::LIFT_WALK_RIGHT;
		}
		else if (KEY_PRESSED(KEY::UP))
		{
			m_CurAS = OguAniState::LIFT_WALK_UP;
		}
		else if (KEY_PRESSED(KEY::DOWN))
		{
			m_CurAS = OguAniState::LIFT_WALK_DOWN;
		}
	}

	if (KEY_RELEASED(KEY::LEFT))
	{
		if (m_CurAS == OguAniState::LIFT_WALK_LEFTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::LIFT_IDLE1_LEFTUP;
					m_StartFrmIdx = 9;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::LIFT_IDLE1_LEFT;
				m_StartFrmIdx = 8;
			}

		}
		else if (m_CurAS == OguAniState::LIFT_WALK_LEFTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::LIFT_IDLE1_LEFTDOWN;
					m_StartFrmIdx = 9;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::LIFT_IDLE1_LEFT;
				m_StartFrmIdx = 8;
			}
		}
		else if (m_CurAS == OguAniState::LIFT_WALK_LEFT)
		{
			m_CurAS = OguAniState::LIFT_IDLE1_LEFT;
			m_StartFrmIdx = 8;
		}
	}

	if (KEY_RELEASED(KEY::RIGHT))
	{
		if (m_CurAS == OguAniState::LIFT_WALK_RIGHTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::LIFT_IDLE1_RIGHTUP;
					m_StartFrmIdx = 9;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::LIFT_IDLE1_RIGHT;
				m_StartFrmIdx = 8;
			}
		}
		else if (m_CurAS == OguAniState::LIFT_WALK_RIGHTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::LIFT_IDLE1_RIGHTDOWN;
					m_StartFrmIdx = 9;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::LIFT_IDLE1_RIGHT;
				m_StartFrmIdx = 8;
			}
		}
		else if (m_CurAS == OguAniState::LIFT_WALK_RIGHT)
		{
			m_CurAS = OguAniState::LIFT_IDLE1_RIGHT;
			m_StartFrmIdx = 8;
		}
	}

	if (KEY_RELEASED(KEY::UP))
	{
		if (m_CurAS == OguAniState::LIFT_WALK_LEFTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::LIFT_IDLE1_LEFTUP;
					m_StartFrmIdx = 9;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::LIFT_IDLE1_UP;
				m_StartFrmIdx = 8;
			}
		}
		else if (m_CurAS == OguAniState::LIFT_WALK_RIGHTUP)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::LIFT_IDLE1_RIGHTUP;
					m_StartFrmIdx = 9;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::LIFT_IDLE1_RIGHT;
				m_StartFrmIdx = 8;
			}
		}
		else if (m_CurAS == OguAniState::LIFT_WALK_UP)
		{
			m_CurAS = OguAniState::LIFT_IDLE1_UP;
			m_StartFrmIdx = 8;
		}
	}

	if (KEY_RELEASED(KEY::DOWN))
	{
		if (m_CurAS == OguAniState::LIFT_WALK_LEFTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::LIFT_IDLE1_LEFTDOWN;
					m_StartFrmIdx = 9;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::LIFT_IDLE1_DOWN;
				m_StartFrmIdx = 8;
			}
		}
		else if (m_CurAS == OguAniState::LIFT_WALK_RIGHTDOWN)
		{
			if (TIME - m_SaveFinalDiagonalTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurAS = OguAniState::LIFT_IDLE1_RIGHTDOWN;
					m_StartFrmIdx = 9;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurAS = OguAniState::LIFT_IDLE1_DOWN;
				m_StartFrmIdx = 8;
			}
		}
		else if (m_CurAS == OguAniState::LIFT_WALK_DOWN)
		{
			m_CurAS = OguAniState::LIFT_IDLE1_DOWN;
			m_StartFrmIdx = 8;
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

void CPlayerScript::LiftEnd()
{
	m_CurPS = PlayerState::LIFT_END;
	m_Interaction = true;
	m_StartFrmIdx = 0;

	switch (m_CurAS)
	{
	case OguAniState::LIFT_IDLE1_DOWN:
	case OguAniState::LIFT_WALK_DOWN:
		m_CurAS = OguAniState::LIFT_IDLE2_DOWN;
		break;
	case OguAniState::LIFT_IDLE1_UP:
	case OguAniState::LIFT_WALK_UP:
		m_CurAS = OguAniState::LIFT_IDLE2_UP;
		break;
	case OguAniState::LIFT_IDLE1_LEFT:
	case OguAniState::LIFT_WALK_LEFT:
		m_CurAS = OguAniState::LIFT_IDLE2_LEFT;
		break;
	case OguAniState::LIFT_IDLE1_RIGHT:
	case OguAniState::LIFT_WALK_RIGHT:
		m_CurAS = OguAniState::LIFT_IDLE2_RIGHT;
		break;
	case OguAniState::LIFT_IDLE1_LEFTDOWN:
	case OguAniState::LIFT_WALK_LEFTDOWN:
		m_CurAS = OguAniState::LIFT_IDLE2_LEFTDOWN;
		break;
	case OguAniState::LIFT_IDLE1_LEFTUP:
	case OguAniState::LIFT_WALK_LEFTUP:
		m_CurAS = OguAniState::LIFT_IDLE2_LEFTUP;
		break;
	case OguAniState::LIFT_IDLE1_RIGHTDOWN:
	case OguAniState::LIFT_WALK_RIGHTDOWN:
		m_CurAS = OguAniState::LIFT_IDLE2_RIGHTDOWN;
		break;
	case OguAniState::LIFT_IDLE1_RIGHTUP:
	case OguAniState::LIFT_WALK_RIGHTUP:
		m_CurAS = OguAniState::LIFT_IDLE2_RIGHTUP;
		break;
	default:
		break;
	}
}
