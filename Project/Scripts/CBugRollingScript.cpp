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
	// ���� Ȱ��ȭ
	if (m_Attack)
	{
		// ������ ���߿��� �ǰ��� �Ұ����ϴ�.
		m_pMonsterScript->SetHitOK(false);

		// �ִϸ��̼� ����
		AniChange();

		// ������ ���°� �ƴϸ� return
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
		// 3�ʸ��� �÷��̾��� ������ �˾Ƴ��� �� �������� ������.
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
	// �÷��̾�� �ε�ĥ ������ ������, ���� �˹�
	if (_OtherObject->GetLayerIdx() == 3)
	{

	}
}

void CBugRollingScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// �浹 ����
	if (_OtherObject->GetLayerIdx() == 7 || _OtherObject->GetLayerIdx() == 8)
	{
		return;
	}

	// ���������� �浹�ǰ� �ϱ�
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
	// ��� ����->������ �غ�
	// ������ �غ�->������
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
	// ���� ���� ���
	Vec3 direction = playerPos - myPos;
	// ���� ���͸� ����ȭ
	direction.Normalize();

	// �밢��
	if (abs(abs(direction.x) - abs(direction.y)) < 0.2f)
	{
		if (direction.x > 0 && direction.y > 0)
		{
			// �ϵ�
			m_CurDir = BugRollingDir::RIGHTUP;
		}
		else if (direction.x < 0 && direction.y < 0)
		{
			// ���� 
			m_CurDir = BugRollingDir::LEFTDOWN;
		}
		else if (direction.x > 0 && direction.y < 0)
		{
			// ����
			m_CurDir = BugRollingDir::RIGHTDOWN;
		}
		else if (direction.x < 0 && direction.y > 0)
		{
			// �ϼ�
			m_CurDir = BugRollingDir::LEFTUP;
		}
	}
	else if (abs(direction.x) >= abs(direction.y))
	{
		if (direction.x > 0)
		{
			// ������
			m_CurDir = BugRollingDir::RIGHT;
		}
		else
		{
			// ����
			m_CurDir = BugRollingDir::LEFT;
		}
	}
	else
	{
		if (direction.y > 0)
		{
			// ��
			m_CurDir = BugRollingDir::UP;
		}
		else
		{
			// �Ʒ�
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
	// ���� Ȱ��ȭ
	m_Attack = true;
	Chase();
	// �� ���� �� ���� �Ȱ��� ���� ������ ����
	// ���� ����Ʈ �Ӹ� ���� ��۹��
	// Ÿ�� ����

	// ������ ���� ��ƼŬ Ȱ��ȭ
}

void CBugRollingScript::Stop()
{
	m_Attack = false;

	// end �ִϸ��̼�
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

	// idle���·� �ǵ�����
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
	// ���� ���� �ı� ����Ʈ ���, �ִϸ��̼� ����
}

