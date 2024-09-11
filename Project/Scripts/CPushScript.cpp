#include "pch.h"
#include "CPushScript.h"

#include "CPlayerScript.h"
#include "CCountDownDeleteScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CPushScript::CPushScript()
	: CScript(UINT(SCRIPT_TYPE::PUSHSCRIPT))
	, m_pPlayer(nullptr)
	, m_pPlayerScript(nullptr)
	, m_Speed(0)
	, m_PlayerCurAS(OguAniState::IDLE)
	, m_IsPush(false)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "m_Speed", &m_Speed);
}

CPushScript::CPushScript(const CPushScript& _Origin)
	: CScript(_Origin)
	, m_pPlayer(_Origin.m_pPlayer)
	, m_pPlayerScript(_Origin.m_pPlayerScript)
	, m_Speed(0)
	, m_PlayerCurAS(OguAniState::IDLE)
	, m_IsPush(false)
{
}

CPushScript::~CPushScript()
{
}

void CPushScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayer = curLevel->FindObjectByName(L"Player");

	CScript* script = m_pPlayer->GetScript("CPlayerScript");
	m_pPlayerScript = dynamic_cast<CPlayerScript*>(script);
}

void CPushScript::Tick()
{
}

void CPushScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{	
	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::STOP)
	{
		return;
	}

	if (m_pPlayer == _OtherObject)
	{
		m_Speed = m_pPlayerScript->GetSpeed() + 100.f;
		m_IsPush = true;			
	}
	else if (_OtherObject->GetLayerIdx() == 6)
	{
		CScript* script = _OtherObject->GetScript("CPushScript");
		CPushScript* pushScript = dynamic_cast<CPushScript*>(script);

		if (pushScript != nullptr)
		{
			// 플레이어가 미는 쪽에서 다른 쪽으로 밀 수 있다는 bool 변수를 전달한다.
			if (m_IsPush)
			{
				pushScript->SetSpeed(m_pPlayerScript->GetSpeed() + 200.f);
				pushScript->SetIsPush(true);
			}
		}
	}
}

void CPushScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::STOP)
	{
		return;
	}
	// 플레이어의 자식이면 return
	if (_OtherObject->GetParent() == m_pPlayer)
	{
		return;
	}

	if (m_pPlayer == _OtherObject)
	{
		m_PlayerCurAS = m_pPlayerScript->GetCurAS();
		Vec3 force = Vec3(0.f, 0.f, 0.f);

		switch (m_PlayerCurAS)
		{
		case OguAniState::PUSH_DOWN:
			force = Vec3(0.f, m_Speed, m_Speed);
			break;
		case OguAniState::PUSH_UP:
			force = Vec3(0.f, -m_Speed, -m_Speed);
			break;
		case OguAniState::PUSH_LEFT:
			force = Vec3(m_Speed, 0.f, 0.f);
			break;
		case OguAniState::PUSH_RIGHT:
			force = Vec3(-m_Speed, 0.f, 0.f);
			break;
		default:
			break;
		}

		GetOwner()->RigidBody()->SetForce(force);
	}
	else if (_OtherObject->GetLayerIdx() == 6 && m_IsPush)
	{
		CScript* script = _OtherObject->GetScript("CPushScript");
		CPushScript* pushScript = dynamic_cast<CPushScript*>(script);

		if (pushScript != nullptr)
		{
			m_PlayerCurAS = m_pPlayerScript->GetCurAS();
			Vec3 force = Vec3(0.f, 0.f, 0.f);

			switch (m_PlayerCurAS)
			{
			case OguAniState::PUSH_DOWN:
				force = Vec3(0.f, m_Speed, m_Speed);
				break;
			case OguAniState::PUSH_UP:
				force = Vec3(0.f, -m_Speed, -m_Speed);
				break;
			case OguAniState::PUSH_LEFT:
				force = Vec3(m_Speed, 0.f, 0.f);
				break;
			case OguAniState::PUSH_RIGHT:
				force = Vec3(-m_Speed, 0.f, 0.f);
				break;
			default:
				break;
			}

			GetOwner()->RigidBody()->SetForce(force);
		}
		else
		{
			m_PlayerCurAS = m_pPlayerScript->GetCurAS();
			Vec3 force = Vec3(0.f, 0.f, 0.f);

			switch (m_PlayerCurAS)
			{
			case OguAniState::PUSH_DOWN:
				force = Vec3(0.f, -m_Speed, -m_Speed);
				break;
			case OguAniState::PUSH_UP:
				force = Vec3(0.f, m_Speed, m_Speed);
				break;
			case OguAniState::PUSH_LEFT:
				force = Vec3(-m_Speed, 0.f, 0.f);
				break;
			case OguAniState::PUSH_RIGHT:
				force = Vec3(m_Speed, 0.f, 0.f);
				break;
			default:
				break;
			}

			GetOwner()->RigidBody()->SetForce(force);
			m_pPlayer->RigidBody()->SetForce(force * 1.5f);
		}
	}
	else if (_OtherObject->GetLayerIdx() == 29 || _OtherObject->GetLayerIdx() == 28)
	{
		m_PlayerCurAS = m_pPlayerScript->GetCurAS();
		Vec3 force = Vec3(0.f, 0.f, 0.f);

		switch (m_PlayerCurAS)
		{
		case OguAniState::PUSH_DOWN:
			force = Vec3(0.f, -m_Speed, -m_Speed);
			break;
		case OguAniState::PUSH_UP:
			force = Vec3(0.f, m_Speed, m_Speed);
			break;
		case OguAniState::PUSH_LEFT:
			force = Vec3(-m_Speed, 0.f, 0.f);
			break;
		case OguAniState::PUSH_RIGHT:
			force = Vec3(m_Speed, 0.f, 0.f);
			break;
		default:
			break;
		}

		GetOwner()->RigidBody()->SetForce(force);
		m_pPlayer->RigidBody()->SetForce(force * 1.5f);
	}
}

void CPushScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 6)
	{
		CScript* script = _OtherObject->GetScript("CPushScript");
		CPushScript* pushScript = dynamic_cast<CPushScript*>(script);

		if (pushScript != nullptr)
		{
			if (m_IsPush)
			{
				pushScript->SetIsPush(false);
			}
		}
	}
	else if (m_pPlayer == _OtherObject)
	{
		m_IsPush = false;
	}
}

void CPushScript::SaveToFile(FILE* _File)
{
}

void CPushScript::LoadFromFile(FILE* _File)
{
}

