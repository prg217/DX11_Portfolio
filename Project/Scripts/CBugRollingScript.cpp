#include "pch.h"
#include "CBugRollingScript.h"

#include "CMonsterScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CBugRollingScript::CBugRollingScript()
	: CScript(UINT(SCRIPT_TYPE::BUGROLLINGSCRIPT))
	, m_pMonsterScript(nullptr)
	, m_pPlayer(nullptr)
	, m_Speed(200.f)
	, m_Attack(false)
	, m_ChaseTime(0.f)
	, m_CurAni(BugRollingAni::IDLE_DOWN)
	, m_CurDir(BugRollingDir::NONE)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::GAMEOBJECT, "Player", &m_pPlayer);
}

CBugRollingScript::CBugRollingScript(const CBugRollingScript& _Origin)
	: CScript(_Origin)
	, m_pMonsterScript(nullptr)
	, m_pPlayer(_Origin.m_pPlayer)
	, m_Speed(_Origin.m_Speed)
	, m_Attack(false)
	, m_ChaseTime(0.f)
	, m_CurAni(BugRollingAni::IDLE_DOWN)
	, m_CurDir(BugRollingDir::NONE)
{
}

CBugRollingScript::~CBugRollingScript()
{
}

void CBugRollingScript::Begin()
{
	m_pMonsterScript = dynamic_cast<CMonsterScript*>(GetOwner()->GetScript("CMonsterScript"));

	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayer = curLevel->FindObjectByName(L"Player");
}

void CBugRollingScript::Tick()
{
	// 공격 활성화
	if (m_Attack)
	{
		// 구르는 도중에는 피격이 불가능하다.
		m_pMonsterScript->SetHitOK(false);

		// 애니메이션 변경
		AniChange();

		// 구르기 상태가 아니면 return
		switch (m_CurAni)
		{
		case BugRollingAni::IDLE_DOWN:
		case BugRollingAni::IDLE_UP:
		case BugRollingAni::IDLE_LEFT:
		case BugRollingAni::IDLE_RIGHT:
		case BugRollingAni::DOWN:
		case BugRollingAni::UP:
		case BugRollingAni::LEFT:
		case BugRollingAni::RIGHT:
		case BugRollingAni::END_DOWN:
		case BugRollingAni::END_UP:
		case BugRollingAni::END_LEFT:
		case BugRollingAni::END_RIGHT:
			return;
			break;
		default:
			break;
		}

		m_ChaseTime += DT;
		// 3초마다 플레이어의 방향을 알아내서 그 방향으로 구른다.
		if (m_ChaseTime >= 3.f)
		{
			m_ChaseTime = 0.f;
			Chase();
		}
		Move();
	}
}

void CBugRollingScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 플레이어와 부딪칠 때마다 데미지, 본인 넉백
	if (_OtherObject->GetLayerIdx() == 3)
	{

	}
}

void CBugRollingScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 충돌 예외
	if (_OtherObject->GetLayerIdx() == 7 || _OtherObject->GetLayerIdx() == 8)
	{
		return;
	}

	// 물리적으로 충돌되게 하기
	Vec3 force = Vec3(0.f, 0.f, 0.f);
	float speed = m_Speed;

	switch (m_CurDir)
	{
	case BugRollingDir::DOWN:
		force = Vec3(0.f, -speed, -speed);
		break;
	case BugRollingDir::UP:
		force = Vec3(0.f, speed, speed);
		break;
	case BugRollingDir::LEFT:
		force = Vec3(-speed, 0.f, 0.f);
		break;
	case BugRollingDir::RIGHT:
		force = Vec3(speed, 0.f, 0.f);
		break;
	case BugRollingDir::LEFTDOWN:
		force = Vec3(-speed, -speed, -speed);
		break;
	case BugRollingDir::LEFTUP:
		force = Vec3(-speed, speed, speed);
		break;
	case BugRollingDir::RIGHTDOWN:
		force = Vec3(speed, -speed, -speed);
		break;
	case BugRollingDir::RIGHTUP:
		force = Vec3(speed, speed, speed);
		break;
	default:
		break;
	}

	GetOwner()->RigidBody()->SetForce(force);
}

void CBugRollingScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugRollingScript::SaveToFile(FILE* _File)
{
}

void CBugRollingScript::LoadFromFile(FILE* _File)
{
}

void CBugRollingScript::AniChange()
{
	// 대기 상태->구르기 준비
	// 구르기 준비->구르기
	switch (m_CurAni)
	{
	case BugRollingAni::IDLE_DOWN:
		m_CurAni = BugRollingAni::DOWN;
		GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);
		break;
	case BugRollingAni::IDLE_UP:
		m_CurAni = BugRollingAni::UP;
		GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);
		break;
	case BugRollingAni::IDLE_LEFT:
		m_CurAni = BugRollingAni::LEFT;
		GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);
		break;
	case BugRollingAni::IDLE_RIGHT:
		m_CurAni = BugRollingAni::RIGHT;
		GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);
		break;
	case BugRollingAni::DOWN:
	case BugRollingAni::UP:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::ROLLING_DOWNUP;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
		}
		break;
	case BugRollingAni::LEFT:
	case BugRollingAni::RIGHT:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::ROLLING_LEFTRIGHT;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
		}
		break;
	default:
		break;
	}

	switch (m_CurAni)
	{
	case BugRollingAni::ROLLING_DOWNUP:
		switch (m_CurDir)
		{
		case BugRollingDir::LEFT:
		case BugRollingDir::RIGHT:
			m_CurAni = BugRollingAni::ROLLING_LEFTRIGHT;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
			break;
		default:
			break;
		}
		break;
	case BugRollingAni::ROLLING_LEFTRIGHT:
		switch (m_CurDir)
		{
		case BugRollingDir::DOWN:
		case BugRollingDir::UP:
			m_CurAni = BugRollingAni::ROLLING_DOWNUP;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CBugRollingScript::Chase()
{
	Vec3 playerPos = m_pPlayer->Transform()->GetRelativePos();
	Vec3 myPos = GetOwner()->Transform()->GetRelativePos();
	// 방향 벡터 계산
	Vec3 direction = playerPos - myPos;
	// 방향 벡터를 정규화
	direction.Normalize();

	// 대각선
	if (abs(abs(direction.x) - abs(direction.y)) < 0.2f)
	{
		if (direction.x > 0 && direction.y > 0)
		{
			// 북동
			m_CurDir = BugRollingDir::RIGHTUP;
		}
		else if (direction.x < 0 && direction.y < 0)
		{
			// 남서 
			m_CurDir = BugRollingDir::LEFTDOWN;
		}
		else if (direction.x > 0 && direction.y < 0)
		{
			// 남동
			m_CurDir = BugRollingDir::RIGHTDOWN;
		}
		else if (direction.x < 0 && direction.y > 0)
		{
			// 북서
			m_CurDir = BugRollingDir::LEFTUP;
		}
	}
	else if (abs(direction.x) >= abs(direction.y))
	{
		if (direction.x > 0)
		{
			// 오른쪽
			m_CurDir = BugRollingDir::RIGHT;
		}
		else
		{
			// 왼쪽
			m_CurDir = BugRollingDir::LEFT;
		}
	}
	else
	{
		if (direction.y > 0)
		{
			// 위
			m_CurDir = BugRollingDir::UP;
		}
		else
		{
			// 아래
			m_CurDir = BugRollingDir::DOWN;
		}
	}
}

void CBugRollingScript::Move()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	switch (m_CurDir)
	{
	case BugRollingDir::DOWN:
		vPos.y -= DT * m_Speed;
		vPos.z -= DT * m_Speed;
		break;
	case BugRollingDir::UP:
		vPos.y += DT * m_Speed;
		vPos.z += DT * m_Speed;
		break;
	case BugRollingDir::LEFT:
		vPos.x -= DT * m_Speed;
		break;
	case BugRollingDir::RIGHT:
		vPos.x += DT * m_Speed;
		break;
	case BugRollingDir::LEFTDOWN:
		vPos.x -= DT * m_Speed;
		vPos.y -= DT * m_Speed;
		vPos.z -= DT * m_Speed;
		break;
	case BugRollingDir::LEFTUP:
		vPos.x -= DT * m_Speed;
		vPos.y += DT * m_Speed;
		vPos.z += DT * m_Speed;
		break;
	case BugRollingDir::RIGHTDOWN:
		vPos.x += DT * m_Speed;
		vPos.y -= DT * m_Speed;
		vPos.z -= DT * m_Speed;
		break;
	case BugRollingDir::RIGHTUP:
		vPos.x += DT * m_Speed;
		vPos.y += DT * m_Speed;
		vPos.z += DT * m_Speed;
		break;
	default:
		break;
	}

	GetOwner()->Transform()->SetRelativePos(vPos);
}

void CBugRollingScript::Attack()
{
	// 공격 활성화
	m_Attack = true;
	Chase();
	// 색 젤리 꽃 빛에 똑같은 색상 맞으면 기절
	// 기절 이펙트 머리 위에 뱅글뱅글
	// 타격 가능

	// 구르는 도중 파티클 활성화
}

void CBugRollingScript::Stop()
{
	m_Attack = false;

	// end 애니메이션
	switch (m_CurDir)
	{
	case BugRollingDir::DOWN:
	case BugRollingDir::LEFTDOWN:
	case BugRollingDir::RIGHTDOWN:
		m_CurAni = BugRollingAni::END_DOWN;
		break;
	case BugRollingDir::UP:
	case BugRollingDir::LEFTUP:
	case BugRollingDir::RIGHTUP:
		m_CurAni = BugRollingAni::END_UP;
		break;
	case BugRollingDir::LEFT:
		m_CurAni = BugRollingAni::END_LEFT;
		break;
	case BugRollingDir::RIGHT:
		m_CurAni = BugRollingAni::END_RIGHT;
		break;
	default:
		break;
	}
	GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);

	m_CurDir = BugRollingDir::NONE;

	// idle상태로 되돌리기
	switch (m_CurAni)
	{
	case BugRollingAni::END_DOWN:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::IDLE_DOWN;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
		}
		break;
	case BugRollingAni::END_UP:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::IDLE_UP;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
		}
		break;
	case BugRollingAni::END_LEFT:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::IDLE_LEFT;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
		}
		break;
	case BugRollingAni::END_RIGHT:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::IDLE_RIGHT;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
		}
		break;
	default:
		break;
	}
}

void CBugRollingScript::Dead()
{
	// 죽을 때는 파괴 이펙트 출력, 애니메이션 없음
}

