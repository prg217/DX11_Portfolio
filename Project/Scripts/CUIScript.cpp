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
	// UI�� �����Ǳ� ������ UI���̾� ���� �ƹ� �ൿ�� ���ϰ� �Ѵ�.
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