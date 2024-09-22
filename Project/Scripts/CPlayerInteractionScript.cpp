#include "pch.h"
#include "CPlayerInteractionScript.h"

#include "CPlayerScript.h"
#include "CLiftScript.h"
#include "CJellyPushScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CPlayerInteractionScript::CPlayerInteractionScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERINTERACTIONSCRIPT))
	, m_pPlayer(nullptr)
	, m_pInteractionObj(nullptr)
{
}

CPlayerInteractionScript::~CPlayerInteractionScript()
{
}

void CPlayerInteractionScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayer = curLevel->FindObjectByName(L"Player");
	m_pPlayer->AddChild(GetOwner());

	CScript* script = m_pPlayer->GetScript("CPlayerScript");
	m_pPlayerScript = dynamic_cast<CPlayerScript*>(script);
}

void CPlayerInteractionScript::Tick()
{
	Move();

	// Lift 중이라면
	if (KEY_TAP(KEY::S) && m_pPlayerScript->GetCurPS() == PlayerState::LIFT_START)
	{
		LiftEnd(m_pInteractionObj);
	}
}

void CPlayerInteractionScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CPlayerInteractionScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 결정키 S
	if (KEY_TAP(KEY::S))
	{
		// 상호작용 오브젝트 레이어
		if (_OtherObject->GetLayerIdx() == 6)
		{
			// CLiftScript 스크립트를 가지고 있다면
			if (_OtherObject->GetScript("CLiftScript") != nullptr)
			{
				Lift(_OtherObject);
			}

			else if (_OtherObject->GetScript("CJellyPushScript") != nullptr)
			{
				CScript* script = _OtherObject->GetScript("CJellyPushScript");
				CJellyPushScript* jellyPushScript = dynamic_cast<CJellyPushScript*>(script);

				// 젤리 스크립트 쪽에 신호를 줘서 분리 시킨 후, 하나를 lift하게 한다.
				Lift(jellyPushScript->Speparation());
			}
		}
	}

}

void CPlayerInteractionScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 상호작용 오브젝트 레이어
	if (_OtherObject->GetLayerIdx() == 6)
	{
		// CPushScript 스크립트를 가지고 있다면
		if (_OtherObject->GetScript("CPushScript") != nullptr)
		{
			m_pPlayerScript->SetIsPush(false);
		}
	}
}

void CPlayerInteractionScript::SaveToFile(FILE* _File)
{
}

void CPlayerInteractionScript::LoadFromFile(FILE* _File)
{
}

void CPlayerInteractionScript::Move()
{
	if (KEY_PRESSED(KEY::DOWN) || KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))
	{
		m_PlayerCurAS = m_pPlayerScript->GetCurAS();
		switch (m_PlayerCurAS)
		{
		case OguAniState::IDLE:
		case OguAniState::WALK_DOWN:
		case OguAniState::RUN_DOWN:
			Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
			Transform()->SetRelativePos(0.f, -0.2f, 0.f);
			break;
		case OguAniState::IDLE_BACK:
		case OguAniState::WALK_UP:
		case OguAniState::RUN_UP:
			Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
			Transform()->SetRelativePos(0.f, 0.2f, 0.f);
			break;
		case OguAniState::IDLE_LEFT:
		case OguAniState::WALK_LEFT:
		case OguAniState::RUN_LEFT:
			Transform()->SetRelativeRotation(0.f, 0.f, 1.571f);
			Transform()->SetRelativePos(-0.2f, 0.f, 0.f);
			break;
		case OguAniState::IDLE_RIGHT:
		case OguAniState::WALK_RIGHT:
		case OguAniState::RUN_RIGHT:
			Transform()->SetRelativeRotation(0.f, 0.f, -1.571f);
			Transform()->SetRelativePos(0.2f, 0.f, 0.f);
			break;
		case OguAniState::IDLE_LEFTDOWN:
		case OguAniState::WALK_LEFTDOWN:
		case OguAniState::RUN_LEFTDOWN:
			Transform()->SetRelativeRotation(0.f, 0.f, 2.356f);
			Transform()->SetRelativePos(-0.2f, -0.2f, 0.f);
			break;
		case OguAniState::IDLE_LEFTUP:
		case OguAniState::WALK_LEFTUP:
		case OguAniState::RUN_LEFTUP:
			Transform()->SetRelativeRotation(0.f, 0.f, 0.785f);
			Transform()->SetRelativePos(-0.2f, 0.2f, 0.f);
			break;
		case OguAniState::IDLE_RIGHTDOWN:
		case OguAniState::WALK_RIGHTDOWN:
		case OguAniState::RUN_RIGHTDOWN:
			Transform()->SetRelativeRotation(0.f, 0.f, -2.356f);
			Transform()->SetRelativePos(0.2f, -0.2f, 0.f);
			break;
		case OguAniState::IDLE_RIGHTUP:
		case OguAniState::WALK_RIGHTUP:
		case OguAniState::RUN_RIGHTUP:
			Transform()->SetRelativeRotation(0.f, 0.f, -0.785f);
			Transform()->SetRelativePos(0.2f, 0.2f, 0.f);
			break;
		default:
			break;
		}
	}
}

void CPlayerInteractionScript::Lift(CGameObject* _Other)
{
	CScript* script = _Other->GetScript("CLiftScript");
	CLiftScript* liftScript = dynamic_cast<CLiftScript*>(script);

	// 플레이어가 아무것도 안 하는 상태인지 체크
	if (m_pPlayerScript->GetCurPS() == PlayerState::NONE)
	{
		m_pInteractionObj = _Other;
		// 들어올리는 애니메이션 재생(재생하는 동안 못 움직임)
		m_pPlayerScript->LiftStart();
		// 자식으로 넣어주고 위치 조정
		liftScript->Start();
	}
}

void CPlayerInteractionScript::LiftEnd(CGameObject* _pInteractionObj)
{
	if (m_pInteractionObj == nullptr)
	{
		return;
	}

	CScript* script = m_pInteractionObj->GetScript("CLiftScript");
	CLiftScript* liftScript = dynamic_cast<CLiftScript*>(script);

	// 내리는 애니메이션 재생(재생하는 동안 못 움직임)
	m_pPlayerScript->LiftEnd();
	liftScript = dynamic_cast<CLiftScript*>(script);
	liftScript->End();
	m_pInteractionObj = nullptr;
}

