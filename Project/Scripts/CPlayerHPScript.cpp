#include "pch.h"
#include "CPlayerHPScript.h"

#include "CHPScript.h"

#include <Engine/CFontMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CPlayerHPScript::CPlayerHPScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERHPSCRIPT))
	, m_PlayerHP(nullptr)
{
}

CPlayerHPScript::~CPlayerHPScript()
{
}

void CPlayerHPScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* player = curLevel->FindObjectByName(L"Player");
	m_PlayerHP = dynamic_cast<CHPScript*>(player->GetScript("CHPScript"));
}

void CPlayerHPScript::Tick()
{
}

void CPlayerHPScript::Render()
{
	if (m_PlayerHP == nullptr)
	{
		return;
	}

	wstring hpText = std::to_wstring(m_PlayerHP->GetHP()) + L" / " + std::to_wstring(m_PlayerHP->GetMaxHP());

	// 두껍게 만들기 위해 여러번 출력
	float x = 185.f;
	float y = 34.0f;
	// 그림자
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x - 3, y, 30, FONT_RGBA(115, 128, 118, 80));
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x + 3, y, 30, FONT_RGBA(115, 128, 118, 80));
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x, y - 3, 30, FONT_RGBA(115, 128, 118, 80));
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x, y + 3, 30, FONT_RGBA(115, 128, 118, 80));

	// 글씨
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x - 1, y, 30, FONT_RGBA(255, 255, 255, 255));
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x + 1, y, 30, FONT_RGBA(255, 255, 255, 255));
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x, y - 1, 30, FONT_RGBA(255, 255, 255, 255));
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x, y + 1, 30, FONT_RGBA(255, 255, 255, 255));
}

void CPlayerHPScript::SaveToFile(FILE* _File)
{
}

void CPlayerHPScript::LoadFromFile(FILE* _File)
{
}

