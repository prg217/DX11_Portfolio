#include "pch.h"
#include "CPlayerDetectScript.h"

#include "CMonsterScript.h"

CPlayerDetectScript::CPlayerDetectScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERDETECTSCRIPT))
{
}

CPlayerDetectScript::CPlayerDetectScript(const CPlayerDetectScript& _Origin)
	: CScript(_Origin)
{
}

CPlayerDetectScript::~CPlayerDetectScript()
{
}

void CPlayerDetectScript::Begin()
{
}

void CPlayerDetectScript::Tick()
{
}

void CPlayerDetectScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 4)
	{
		CMonsterScript* script = dynamic_cast<CMonsterScript*>(_OtherObject->GetScript("CMonsterScript"));
		if (script != nullptr)
		{
			script->PlayerDetect(true);
		}
	}
}

void CPlayerDetectScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerDetectScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 4)
	{
		CMonsterScript* script = dynamic_cast<CMonsterScript*>(_OtherObject->GetScript("CMonsterScript"));
		if (script != nullptr)
		{
			script->PlayerDetect(false);
		}
	}
}

void CPlayerDetectScript::SaveToFile(FILE* _File)
{
}

void CPlayerDetectScript::LoadFromFile(FILE* _File)
{
}
