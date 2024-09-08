#include "pch.h"
#include "CCameraPlayerTrackingScript.h"

#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CCameraPlayerTrackingScript::CCameraPlayerTrackingScript()
	: CScript(UINT(SCRIPT_TYPE::CAMERAPLAYERTRACKINGSCRIPT))
	, m_IsMove(true)
	, m_pPlayer(nullptr)
{
}

CCameraPlayerTrackingScript::~CCameraPlayerTrackingScript()
{
}

void CCameraPlayerTrackingScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayer = curLevel->FindObjectByName(L"Player");
}

void CCameraPlayerTrackingScript::Tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
	{
		OrthoGraphicMove();
	}

	else if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	{
		PerspectiveMove();
	}


	if (KEY_TAP(KEY::P))
	{
		if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}
}

void CCameraPlayerTrackingScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCameraPlayerTrackingScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CCameraPlayerTrackingScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCameraPlayerTrackingScript::SaveToFile(FILE* _File)
{
}

void CCameraPlayerTrackingScript::LoadFromFile(FILE* _File)
{
}

void CCameraPlayerTrackingScript::OrthoGraphicMove()
{
	// �÷��̾ �������, ���� �� �ٱ����� �������ϸ� �ű� ���� �ִ� �ݶ��̴��� �� �̻� ī�޶� �̵��� ����
	Vec3 playerPos = m_pPlayer->Transform()->GetRelativePos();
	Vec3 pos;
	if (m_IsMove)
	{
		pos = Vec3(playerPos.x, playerPos.y, 0.f);
	}
	else
	{
		Vec3 myPos = GetOwner()->Transform()->GetRelativePos();
		switch (m_Dir)
		{
		case CameraDontMove::LEFT:
			// ���� ��ǥ���� ���������� ���� �̵� ����
			if (playerPos.x - myPos.x > 0)
			{
				pos = Vec3(playerPos.x, myPos.y, 0.f);
			}
			else
			{
				pos = myPos;
			}
			break;
		case CameraDontMove::RIGHT:
			if (playerPos.x - myPos.x < 0)
			{
				pos = Vec3(playerPos.x, myPos.y, 0.f);
			}
			else
			{
				pos = myPos;
			}
			break;
		case CameraDontMove::UP:
			if (playerPos.y - myPos.y > 0)
			{
				pos = Vec3(myPos.x, playerPos.y, 0.f);
			}
			else
			{
				pos = myPos;
			}
			break;
		case CameraDontMove::DOWN:
			if (playerPos.y - myPos.y < 0)
			{
				pos = Vec3(myPos.x, playerPos.y, 0.f);
			}
			else
			{
				pos = myPos;
			}
			break;
		default:
			break;
		}
	}
	GetOwner()->Transform()->SetRelativePos(pos);
}

void CCameraPlayerTrackingScript::PerspectiveMove()
{
}