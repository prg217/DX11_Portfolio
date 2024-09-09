#include "pch.h"
#include "CPlayerCameraScript.h"

#include "CCameraPlayerTrackingScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CPlayerCameraScript::CPlayerCameraScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERCAMERASCRIPT))
	, m_pCamera(nullptr)
	, m_pCameraScript(nullptr)
{
}

CPlayerCameraScript::~CPlayerCameraScript()
{
}

void CPlayerCameraScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* pPlayer = curLevel->FindObjectByName(L"Player");
	pPlayer->AddChild(GetOwner());

	m_pCamera = curLevel->FindObjectByName(L"MainCamera");

	CScript* script = m_pCamera->GetScript("CCameraPlayerTrackingScript");
	m_pCameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
}

void CPlayerCameraScript::Tick()
{
}

void CPlayerCameraScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (curLevel->GetState() == LEVEL_STATE::STOP)
	{
		return;
	}

	if (_OtherObject->GetName() == L"Lblock")
	{
		m_pCameraScript->Stop(CameraDontMove::LEFT);
	}
	else if (_OtherObject->GetName() == L"Rblock")
	{
		m_pCameraScript->Stop(CameraDontMove::RIGHT);
	}
	else if (_OtherObject->GetName() == L"Ublock")
	{
		m_pCameraScript->Stop(CameraDontMove::UP);
	}
	else if (_OtherObject->GetName() == L"Dblock")
	{
		m_pCameraScript->Stop(CameraDontMove::DOWN);
	}
}

void CPlayerCameraScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerCameraScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (curLevel->GetState() == LEVEL_STATE::STOP)
	{
		return;
	}
	if (GetOwner()->Collider2D()->GetOverlapCount() == 0)
	{
		m_pCameraScript->Move();
	}
}

void CPlayerCameraScript::SaveToFile(FILE* _File)
{
}

void CPlayerCameraScript::LoadFromFile(FILE* _File)
{
}
