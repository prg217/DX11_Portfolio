#include "pch.h"
#include "CCameraPlayerTrackingScript.h"

#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include <random>

CCameraPlayerTrackingScript::CCameraPlayerTrackingScript()
	: CScript(UINT(SCRIPT_TYPE::CAMERAPLAYERTRACKINGSCRIPT))
	, m_IsMove(true)
	, m_Speed(2000.f)
	, m_pFocusObj(nullptr)
	, m_Shaking(false)
	, m_ShakingTime(0.f)
{
}

CCameraPlayerTrackingScript::~CCameraPlayerTrackingScript()
{
}

void CCameraPlayerTrackingScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pFocusObj = curLevel->FindObjectByName(L"Player");
}

void CCameraPlayerTrackingScript::Tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
	{
		OrthoGraphicMove();

		if (m_Shaking)
		{
			m_ShakingTime += DT;

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> distrib(-15.f, 15.f);  // 흔들림 범위

			if (m_ShakingTime < 0.3f)
			{
				// 노이즈 생성 (매 프레임마다, 각 축에 대해 랜덤한 오프셋 추가)
				float offsetX = distrib(gen);
				float offsetY = distrib(gen);
				float offsetZ = distrib(gen);

				// 카메라 위치에 흔들림 적용
				Vec3 pos = GetOwner()->Transform()->GetRelativePos();
				Vec3 shakeOffset = Vec3(offsetX, offsetY, offsetZ);
				pos += shakeOffset;
				GetOwner()->Transform()->SetRelativePos(pos);
			}
			else
			{
				m_Shaking = false;
				m_ShakingTime = 0.f;
			}
		}
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

void CCameraPlayerTrackingScript::Shaking()
{
	m_Shaking = true;
}

void CCameraPlayerTrackingScript::OrthoGraphicMove()
{
	// 플레이어를 따라오되, 이제 맵 바깥으로 나가려하면 거기 막고 있는 콜라이더가 더 이상 카메라 이동을 막음
	Vec3 playerPos = m_pFocusObj->Transform()->GetRelativePos();
	Vec3 myPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 pos;
	if (m_IsMove)
	{
		// 방향 벡터 계산
		Vec3 direction = playerPos - myPos;
		// 방향 벡터를 정규화
		direction.Normalize();

		myPos.x += direction.x * m_Speed * DT;
		myPos.y += direction.y * m_Speed * DT;
		pos = myPos;
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
			{
				float posY = myPos.y + (direction.y * m_Speed * DT);
				pos = Vec3(myPos.x, posY, 0.f);
			}
				break;
			case CameraDontMove::RIGHT:
			{
				float posY = myPos.y + (direction.y * m_Speed * DT);
				pos = Vec3(myPos.x, posY, 0.f);
			}
				break;
			case CameraDontMove::UP:
			{
				float posX = myPos.x + (direction.x * m_Speed * DT);
				pos = Vec3(posX, myPos.y, 0.f);
			}
				break;
			case CameraDontMove::DOWN:
			{
				float posX = myPos.x + (direction.x * m_Speed * DT);
				pos = Vec3(posX, myPos.y, 0.f);
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