#include "pch.h"
#include "CSwingObjScript.h"

#include "CPlayerScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CSwingObjScript::CSwingObjScript()
	: CScript(UINT(SCRIPT_TYPE::SWINGOBJSCRIPT))
	, m_pPlayer(nullptr)
	, m_pPlayerScript(nullptr)
	, m_IsSwing(false)
{

}

CSwingObjScript::~CSwingObjScript()
{
}

void CSwingObjScript::Begin()
{
	GetRenderComponent()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	GetRenderComponent()->GetDynamicMaterial();

	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayer = curLevel->FindObjectByName(L"Player");
	m_pPlayer->AddChild(GetOwner());

	CScript* script = m_pPlayer->GetScript("CPlayerScript");
	m_pPlayerScript = dynamic_cast<CPlayerScript*>(script);


}

void CSwingObjScript::Tick()
{
	// 채 휘두르는 신호 올 때마다 오구 상태에 따른 위치, 회전 변경 및 애니메이션 재생
	if (!m_IsSwing)
	{
		m_PlayerCurAS = m_pPlayerScript->GetCurAS();
		switch (m_PlayerCurAS)
		{
		case OguAniState::SWING_DOWN:
			Transform()->SetRelativeScale(-1.f, -1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
			Transform()->SetRelativePos(0.f, -0.5f, 0.f);
			break;
		case OguAniState::SWING_UP:
			Transform()->SetRelativeScale(1.f, 1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
			Transform()->SetRelativePos(0.f, 0.5f, 0.f);
			break;
		case OguAniState::SWING_LEFT:
			Transform()->SetRelativeScale(1.f, 1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, 1.571f);
			Transform()->SetRelativePos(-0.5f, 0.f, 0.f);
			break;
		case OguAniState::SWING_RIGHT:
			Transform()->SetRelativeScale(1.f, 1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, -1.571f);
			Transform()->SetRelativePos(0.5f, 0.f, 0.f);
			break;
		case OguAniState::SWING_LEFTDOWN:
			Transform()->SetRelativeScale(1.f, 1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, 2.356f);
			Transform()->SetRelativePos(-0.3f, -0.3f, 0.f);
			break;
		case OguAniState::SWING_LEFTUP:
			Transform()->SetRelativeScale(1.f, 1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, 0.785f);
			Transform()->SetRelativePos(-0.3f, 0.3f, 0.f);
			break;
		case OguAniState::SWING_RIGHTDOWN:
			Transform()->SetRelativeScale(1.f, 1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, -2.356f);
			Transform()->SetRelativePos(0.3f, -0.3f, 0.f);
			break;
		case OguAniState::SWING_RIGHTUP:
			Transform()->SetRelativeScale(1.f, 1.f, 0.f);
			Transform()->SetRelativeRotation(0.f, 0.f, -0.785f);
			Transform()->SetRelativePos(0.3f, 0.3f, 0.f);
			break;
		default:
			break;
		}

		switch (m_PlayerCurAS)
		{
		case OguAniState::SWING_DOWN:
		case OguAniState::SWING_UP:
		case OguAniState::SWING_LEFT:
		case OguAniState::SWING_RIGHT:
		case OguAniState::SWING_LEFTDOWN:
		case OguAniState::SWING_LEFTUP:
		case OguAniState::SWING_RIGHTDOWN:
		case OguAniState::SWING_RIGHTUP:
			FlipBookComponent()->Reset();
			FlipBookComponent()->Play(0, 6, false);
			m_IsSwing = true;
			break;
		default:
			break;
		}
	}

	if (FlipBookComponent()->GetIsFinish())
	{
		m_IsSwing = false;
		//FlipBookComponent()->Clear();
	}
}

void CSwingObjScript::SaveToFile(FILE* _File)
{
}

void CSwingObjScript::LoadFromFile(FILE* _File)
{
}