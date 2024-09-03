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
	, m_PlayerSpeed(0)
	, m_PlayerCurAS(OguAniState::IDLE)
{
}

CPushScript::CPushScript(const CPushScript& _Origin)
	: CScript(_Origin)
	, m_pPlayer(_Origin.m_pPlayer)
	, m_pPlayerScript(_Origin.m_pPlayerScript)
	, m_PlayerSpeed(0)
	, m_PlayerCurAS(OguAniState::IDLE)
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
	if (m_pPlayer == _OtherObject)
	{
		m_PlayerSpeed = m_pPlayerScript->GetSpeed() - 100.f;
	}

}

void CPushScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (m_pPlayer == _OtherObject)
	{
		m_PlayerCurAS = m_pPlayerScript->GetCurAS();
		Vec3 force = Vec3(0.f, 0.f, 0.f);

		switch (m_PlayerCurAS)
		{
		case OguAniState::PUSH_DOWN:
			force = Vec3(0.f, m_PlayerSpeed, 0.f);
			break;
		case OguAniState::PUSH_UP:
			force = Vec3(0.f, -m_PlayerSpeed, 0.f);
			break;
		case OguAniState::PUSH_LEFT:
			force = Vec3(m_PlayerSpeed, 0.f, 0.f);
			break;
		case OguAniState::PUSH_RIGHT:
			force = Vec3(-m_PlayerSpeed, 0.f, 0.f);
			break;
		default:
			break;
		}

		GetOwner()->RigidBody()->SetForce(force);
	}
}

void CPushScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPushScript::SaveToFile(FILE* _File)
{
}

void CPushScript::LoadFromFile(FILE* _File)
{
}

