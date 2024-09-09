#include "pch.h"
#include "CCameraPlayerTrackingScript.h"

#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CCameraPlayerTrackingScript::CCameraPlayerTrackingScript()
	: CScript(UINT(SCRIPT_TYPE::CAMERAPLAYERTRACKINGSCRIPT))
	, m_IsMove(true)
	, m_Speed(20.f)
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
	// 플레이어를 따라오되, 이제 맵 바깥으로 나가려하면 거기 막고 있는 콜라이더가 더 이상 카메라 이동을 막음
	Vec3 playerPos = m_pPlayer->Transform()->GetRelativePos();
	Vec3 myPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 pos;
	if (m_IsMove)
	{
		// 방향 벡터 계산
		Vec3 direction = playerPos - myPos;
		pos = myPos;
		if ((direction.x <= -1.f || direction.x >= 1.f))
		{
			// 방향 벡터를 정규화
			direction.Normalize();

			// 속도를 곱하여 위치 업데이트
			myPos.x = myPos.x + (direction.x * m_Speed);
			pos = myPos;
		}

		if ((direction.y <= -1.f || direction.y >= 1.f))
		{
			// 방향 벡터를 정규화
			direction.Normalize();

			// 속도를 곱하여 위치 업데이트
			myPos.y = myPos.y + (direction.y * m_Speed);
			pos = myPos;
		}
	}
	else
	{
		Vec3 direction = playerPos - myPos;
		if ((direction.x <= -3.f || direction.x >= 3.f) || (direction.y <= -3.f || direction.y >= 3.f))
		{
			direction.Normalize();
			switch (m_Dir)
			{
			case CameraDontMove::LEFT:
				// 기존 좌표보다 오른쪽으로 가면 이동 가능
				if ((playerPos.x - myPos.x > 0))
				{
					float posY = myPos.y + (direction.y * m_Speed);
					pos = Vec3(posY, playerPos.y, 0.f);
				}
				else
				{
					pos = myPos;
				}
				break;
			case CameraDontMove::RIGHT:
				if (playerPos.x - myPos.x < 0)
				{
					float posY = myPos.y + (direction.y * m_Speed);
					pos = Vec3(posY, playerPos.y, 0.f);
				}
				else
				{
					pos = myPos;
				}
				break;
			case CameraDontMove::UP:
				if (playerPos.y - myPos.y > 0)
				{
					float posX = myPos.x + (direction.x * m_Speed);
					pos = Vec3(posX, myPos.y, 0.f);
				}
				else
				{
					pos = myPos;
				}
				break;
			case CameraDontMove::DOWN:
				if (playerPos.y - myPos.y < 0)
				{
					float posX = myPos.x + (direction.x * m_Speed);
					pos = Vec3(posX, myPos.y, 0.f);
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
		else
		{
			pos = myPos;
		}
	}
		
	GetOwner()->Transform()->SetRelativePos(pos);
}

void CCameraPlayerTrackingScript::PerspectiveMove()
{
}