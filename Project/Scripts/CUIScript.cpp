#include "pch.h"
#include "CUIScript.h"

CUIScript::CUIScript()
	: CScript(UINT(SCRIPT_TYPE::UISCRIPT))
{
}

CUIScript::CUIScript(const CUIScript& _Origin)
	: CScript(_Origin)
{
}

CUIScript::~CUIScript()
{
	GetOwner()->SetUIActive(false);
}

void CUIScript::Begin()
{

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

void CUIScript::UIActive(bool _Active)
{
	// UI가 비활성화되기 전에는 UI레이어 제외 아무 행동도 못하게 한다.
	GetOwner()->SetUIActive(_Active);
}
