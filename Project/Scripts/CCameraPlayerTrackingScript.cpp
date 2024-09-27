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
			std::uniform_real_distribution<float> distrib(-15.f, 15.f);  // ��鸲 ����

			if (m_ShakingTime < 0.3f)
			{
				// ������ ���� (�� �����Ӹ���, �� �࿡ ���� ������ ������ �߰�)
				float offsetX = distrib(gen);
				float offsetY = distrib(gen);
				float offsetZ = distrib(gen);

				// ī�޶� ��ġ�� ��鸲 ����
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
	// �÷��̾ �������, ���� �� �ٱ����� �������ϸ� �ű� ���� �ִ� �ݶ��̴��� �� �̻� ī�޶� �̵��� ����
	Vec3 playerPos = m_pFocusObj->Transform()->GetRelativePos();
	Vec3 myPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 pos;
	if (m_IsMove)
	{
		// ���� ���� ���
		Vec3 direction = playerPos - myPos;
		// ���� ���͸� ����ȭ
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