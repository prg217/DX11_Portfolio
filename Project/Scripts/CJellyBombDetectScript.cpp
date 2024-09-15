#include "pch.h"
#include "CJellyBombDetectScript.h"

#include "CStoneBreakableScript.h"

CJellyBombDetectScript::CJellyBombDetectScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYBOMBDETECTSCRIPT))
{
}

CJellyBombDetectScript::CJellyBombDetectScript(const CJellyBombDetectScript& _Origin)
	: CScript(_Origin)
{
}

CJellyBombDetectScript::~CJellyBombDetectScript()
{
}

void CJellyBombDetectScript::Begin()
{
}

void CJellyBombDetectScript::Tick()
{
}

void CJellyBombDetectScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 28)
	{
		if (_OtherObject->GetScript("CStoneBreakableScript") != nullptr)
		{
			CStoneBreakableScript* script = dynamic_cast<CStoneBreakableScript*>(_OtherObject->GetScript("CStoneBreakableScript"));
			script->Destroy();
		}
	}
}

void CJellyBombDetectScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyBombDetectScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyBombDetectScript::SaveToFile(FILE* _File)
{
}

void CJellyBombDetectScript::LoadFromFile(FILE* _File)
{
}
