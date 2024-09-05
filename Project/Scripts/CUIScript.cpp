#include "pch.h"
#include "CUIScript.h"

CUIScript::CUIScript()
	: CScript(UINT(SCRIPT_TYPE::TEXTBOXSCRIPT))
{
}

CUIScript::CUIScript(const CUIScript& _Origin)
	: CScript(_Origin)
{
}

CUIScript::~CUIScript()
{
}

void CUIScript::Begin()
{
	// UI가 삭제되기 전에는 UI레이어 제외 아무 행동도 못하게 한다.
	GetOwner()->SetUIActive(true);
}

void CUIScript::Tick()
{
}

void CUIScript::SaveToFile(FILE* _File)
{
}

void CUIScript::LoadFromFile(FILE* _File)
{
}