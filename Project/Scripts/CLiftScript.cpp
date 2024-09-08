#include "pch.h"
#include "CLiftScript.h"

#include "CPlayerScript.h"
#include "CCountDownDeleteScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

CLiftScript::CLiftScript()
	: CScript(UINT(SCRIPT_TYPE::LIFTSCRIPT))
	, m_pPlayer(nullptr)
	, m_pPlayerScript(nullptr)
	, m_Start(false)
	, m_End(false)
{
}

CLiftScript::CLiftScript(const CLiftScript& _Origin)
	: CScript(_Origin)
	, m_pPlayer(_Origin.m_pPlayer)
	, m_pPlayerScript(_Origin.m_pPlayerScript)
	, m_Start(false)
	, m_End(false)
{
}

CLiftScript::~CLiftScript()
{
}

void CLiftScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayer = curLevel->FindObjectByName(L"Player");

	CScript* script = m_pPlayer->GetScript("CPlayerScript");
	m_pPlayerScript = dynamic_cast<CPlayerScript*>(script);
}

void CLiftScript::Tick()
{
	if (m_Start)
	{
		if (m_pPlayer->FlipBookComponent()->GetCurIdx() == 4)
		{
			// 중간 위치
			m_pPlayer->AddChild(GetOwner());

			m_PlayerCurAS = m_pPlayerScript->GetCurAS();
			Vec3 playerPos = m_pPlayer->Transform()->GetRelativePos();
			float addPos = 30.f;
			switch (m_PlayerCurAS)
			{
			case OguAniState::LIFT_IDLE1_DOWN:
			case OguAniState::LIFT_WALK_DOWN:
				GetOwner()->Transform()->SetRelativePos(Vec3(0.f, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE1_UP:
			case OguAniState::LIFT_WALK_UP:
				GetOwner()->Transform()->SetRelativePos(Vec3(0.f, addPos, addPos));
				break;
			case OguAniState::LIFT_IDLE1_LEFT:
			case OguAniState::LIFT_WALK_LEFT:
				GetOwner()->Transform()->SetRelativePos(Vec3(-addPos, 0.f, 0.f));
				break;
			case OguAniState::LIFT_IDLE1_RIGHT:
			case OguAniState::LIFT_WALK_RIGHT:
				GetOwner()->Transform()->SetRelativePos(Vec3(addPos, 0.f, 0.f));
				break;
			case OguAniState::LIFT_IDLE1_LEFTDOWN:
			case OguAniState::LIFT_WALK_LEFTDOWN:
				GetOwner()->Transform()->SetRelativePos(Vec3(-addPos, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE1_LEFTUP:
			case OguAniState::LIFT_WALK_LEFTUP:
				GetOwner()->Transform()->SetRelativePos(Vec3(-addPos, addPos, addPos));
				break;
			case OguAniState::LIFT_IDLE1_RIGHTDOWN:
			case OguAniState::LIFT_WALK_RIGHTDOWN:
				GetOwner()->Transform()->SetRelativePos(Vec3(addPos, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE1_RIGHTUP:
			case OguAniState::LIFT_WALK_RIGHTUP:
				GetOwner()->Transform()->SetRelativePos(Vec3(addPos, addPos, addPos));
				break;
			default:
				break;
			}
		}
		if (m_pPlayer->FlipBookComponent()->GetIsFinish())
		{
			GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 50.f, 50.f));
			m_Start = false;
		}
	}

	if (m_End)
	{
		// 내려놓기
		if (m_pPlayer->FlipBookComponent()->GetCurIdx() == 1)
		{
			// 중간 위치
			m_PlayerCurAS = m_pPlayerScript->GetCurAS();
			Vec3 playerPos = m_pPlayer->Transform()->GetRelativePos();
			float addPos = 30.f;
			switch (m_PlayerCurAS)
			{
			case OguAniState::LIFT_IDLE2_DOWN:
				GetOwner()->Transform()->SetRelativePos(Vec3(0.f, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE2_UP:
				GetOwner()->Transform()->SetRelativePos(Vec3(0.f, addPos, addPos));
				break;
			case OguAniState::LIFT_IDLE2_LEFT:
				GetOwner()->Transform()->SetRelativePos(Vec3(-addPos, 0.f, 0.f));
				break;
			case OguAniState::LIFT_IDLE2_RIGHT:
				GetOwner()->Transform()->SetRelativePos(Vec3(addPos, 0.f, 0.f));
				break;
			case OguAniState::LIFT_IDLE2_LEFTDOWN:
				GetOwner()->Transform()->SetRelativePos(Vec3(-addPos, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE2_LEFTUP:
				GetOwner()->Transform()->SetRelativePos(Vec3(-addPos, addPos, addPos));
				break;
			case OguAniState::LIFT_IDLE2_RIGHTDOWN:
				GetOwner()->Transform()->SetRelativePos(Vec3(addPos, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE2_RIGHTUP:
				GetOwner()->Transform()->SetRelativePos(Vec3(addPos, addPos, addPos));
				break;
			default:
				break;
			}
		}
		if (m_pPlayer->FlipBookComponent()->GetIsFinish())
		{
			// 내려놓기
			// 자식 제외
			GetOwner()->DeregisterChild();
			CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CLayer* pLayer = pLevel->GetLayer(6);
			pLayer->RegisterAsParent(GetOwner());

			m_PlayerCurAS = m_pPlayerScript->GetCurAS();
			Vec3 playerPos = m_pPlayer->Transform()->GetRelativePos();
			float addPos = 50.f;
			switch (m_PlayerCurAS)
			{
			case OguAniState::LIFT_IDLE2_DOWN:
			case OguAniState::IDLE:
				GetOwner()->Transform()->SetRelativePos(playerPos + Vec3(0.f, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE2_UP:
			case OguAniState::IDLE_BACK:
				GetOwner()->Transform()->SetRelativePos(playerPos + Vec3(0.f, addPos, addPos));
				break;
			case OguAniState::LIFT_IDLE2_LEFT:
			case OguAniState::IDLE_LEFT:
				GetOwner()->Transform()->SetRelativePos(playerPos + Vec3(-addPos, 0.f, 0.f));
				break;
			case OguAniState::LIFT_IDLE2_RIGHT:
			case OguAniState::IDLE_RIGHT:
				GetOwner()->Transform()->SetRelativePos(playerPos + Vec3(addPos, 0.f, 0.f));
				break;
			case OguAniState::LIFT_IDLE2_LEFTDOWN:
			case OguAniState::IDLE_LEFTDOWN:
				GetOwner()->Transform()->SetRelativePos(playerPos + Vec3(-addPos, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE2_LEFTUP:
			case OguAniState::IDLE_LEFTUP:
				GetOwner()->Transform()->SetRelativePos(playerPos + Vec3(-addPos, addPos, addPos));
				break;
			case OguAniState::LIFT_IDLE2_RIGHTDOWN:
			case OguAniState::IDLE_RIGHTDOWN:
				GetOwner()->Transform()->SetRelativePos(playerPos + Vec3(addPos, -addPos, -addPos));
				break;
			case OguAniState::LIFT_IDLE2_RIGHTUP:
			case OguAniState::IDLE_RIGHTUP:
				GetOwner()->Transform()->SetRelativePos(playerPos + Vec3(addPos, addPos, addPos));
				break;
			default:
				break;
			}

			// 파티클 소환
			CGameObject* pParticleObj = new CGameObject;
			pParticleObj->SetName(L"LiftParticle");

			pParticleObj->AddComponent(new CTransform);
			pParticleObj->AddComponent(new CParticleSystem);

			wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
			strInitPath += L"particle\\lift.particle";

			FILE* File = nullptr;
			_wfopen_s(&File, strInitPath.c_str(), L"rb");

			pParticleObj->ParticleSystem()->LoadFromFile(File);
			fclose(File);

			// 삭제되는 시간을 지정해준다.
			pParticleObj->AddComponent(new CCountDownDeleteScript);
			CScript* pScript = pParticleObj->GetScript("CCountDownDeleteScript");
			CCountDownDeleteScript* pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
			pCountDown->SetSaveTime(TIME);
			pCountDown->SetDeadTime(4.f);

			Vec3 spawnPos = GetOwner()->Transform()->GetRelativePos();

			pParticleObj->Transform()->SetRelativePos(spawnPos);

			CreateObject(pParticleObj, 0);

			m_End = false;
		}
	}
}

void CLiftScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CLiftScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CLiftScript::SaveToFile(FILE* _File)
{
}

void CLiftScript::LoadFromFile(FILE* _File)
{
}

void CLiftScript::Start()
{
	m_Start = true;
	GetOwner()->SpriteComponent()->SetOutline(false);
}

void CLiftScript::End()
{
	m_End = true;
}
