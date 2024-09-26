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
	wstring fontName = L"ī��24 �����";

	// �β��� ����� ���� ������ ���
	float x = 185.f;
	float y = 34.0f;
	// �׸���
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x - 3, y, 30, FONT_RGBA(115, 128, 118, 80), fontName);
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x + 3, y, 30, FONT_RGBA(115, 128, 118, 80), fontName);
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x, y - 3, 30, FONT_RGBA(115, 128, 118, 80), fontName);
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x, y + 3, 30, FONT_RGBA(115, 128, 118, 80), fontName);

	// �۾�
	CFontMgr::GetInst()->DrawCenterFont(hpText.c_str(), x, y, 30, FONT_RGBA(255, 255, 255, 255), fontName);
}

void CPlayerHPScript::SaveToFile(FILE* _File)
{
}

void CPlayerHPScript::LoadFromFile(FILE* _File)
{
}

