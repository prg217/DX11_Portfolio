#include "pch.h"
#include "CNPCScript.h"

#include "CTextBoxScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CNPCScript::CNPCScript()
	: CScript(UINT(SCRIPT_TYPE::NPCSCRIPT))
	, m_NPCName{}
	, m_pTextBox(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::WSTRING, "NPC Name", &m_NPCName);
}

CNPCScript::CNPCScript(const CNPCScript& _Origin)
	: CScript(_Origin)
	, m_NPCName{}
	, m_pTextBox(nullptr)
{
}

CNPCScript::~CNPCScript()
{
}

void CNPCScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pTextBox = curLevel->FindObjectByName(L"TextBox");
	m_pTextBoxName = curLevel->FindObjectByName(L"TextBoxName");

	CScript* script = m_pTextBox->GetScript("CTextBoxScript");
	m_pTextBoxScript = dynamic_cast<CTextBoxScript*>(script);
	script = m_pTextBoxName->GetScript("CTextBoxScript");
	m_pTextBoxNameScript = dynamic_cast<CTextBoxScript*>(script);
}

void CNPCScript::Tick()
{
}

void CNPCScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CNPCScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (GetOwner()->GetUIActive())
	{
		return;
	}

	// 플레이어 상호작용과 오버랩인데 S키가 눌렸다면 대화창
	if (_OtherObject->GetLayerIdx() == 5)
	{
		if (KEY_TAP(KEY::S))
		{
			ActiveTextBox();
		}
	}
}

void CNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CNPCScript::SaveToFile(FILE* _File)
{
}

void CNPCScript::LoadFromFile(FILE* _File)
{
}

void CNPCScript::ActiveTextBox()
{
	m_pTextBoxNameScript->IsName();
	m_pTextBoxNameScript->SetName(m_NPCName);

	m_pTextBoxScript->Active(true);
}

void CNPCScript::SetName(wstring _Text)
{
	m_NPCName = _Text;
}

void CNPCScript::LoadText(const wstring& _FileName)
{
	m_pTextBoxScript->LoadText(_FileName);
}