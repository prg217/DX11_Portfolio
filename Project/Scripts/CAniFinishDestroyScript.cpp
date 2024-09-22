#include "pch.h"
#include "CAniFinishDestroyScript.h"

CAniFinishDestroyScript::CAniFinishDestroyScript()
	: CScript(UINT(SCRIPT_TYPE::ANIFINISHDESTROYSCRIPT))
{
}

CAniFinishDestroyScript::CAniFinishDestroyScript(const CAniFinishDestroyScript& _Origin)
	: CScript(_Origin)
{
}

CAniFinishDestroyScript::~CAniFinishDestroyScript()
{
}

void CAniFinishDestroyScript::Begin()
{
}

void CAniFinishDestroyScript::Tick()
{
	if (GetOwner()->FlipBookComponent()->GetIsFinish())
	{
		DeleteObject(GetOwner());
	}
}

void CAniFinishDestroyScript::SaveToFile(FILE* _File)
{
}

void CAniFinishDestroyScript::LoadFromFile(FILE* _File)
{
}