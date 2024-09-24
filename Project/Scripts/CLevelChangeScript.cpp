#include "pch.h"
#include "CLevelChangeScript.h"

CLevelChangeScript::CLevelChangeScript()
	: CScript(UINT(SCRIPT_TYPE::LEVELCHANGESCRIPT))
{
}

CLevelChangeScript::CLevelChangeScript(const CLevelChangeScript& _Origin)
	: CScript(_Origin)
{
}

CLevelChangeScript::~CLevelChangeScript()
{
}

void CLevelChangeScript::Begin()
{
}

void CLevelChangeScript::Tick()
{
}

void CLevelChangeScript::Render()
{
}

void CLevelChangeScript::SaveToFile(FILE* _File)
{
}

void CLevelChangeScript::LoadFromFile(FILE* _File)
{
}

void CLevelChangeScript::ChangeLevel()
{
}
