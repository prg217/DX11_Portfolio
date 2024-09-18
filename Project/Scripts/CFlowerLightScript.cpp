#include "pch.h"
#include "CFlowerLightScript.h"

CFlowerLightScript::CFlowerLightScript()
	: CScript(UINT(SCRIPT_TYPE::FLOWERLIGHTSCRIPT))
{
}

CFlowerLightScript::CFlowerLightScript(const CFlowerLightScript& _Origin)
	: CScript(_Origin)
{
}

CFlowerLightScript::~CFlowerLightScript()
{
}

void CFlowerLightScript::Begin()
{
}

void CFlowerLightScript::Tick()
{
}

void CFlowerLightScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightScript::SaveToFile(FILE* _File)
{
}

void CFlowerLightScript::LoadFromFile(FILE* _File)
{
}

void CFlowerLightScript::Bloom()
{
	GetOwner()->FlipBookComponent()->Play(0, 10, false);
}

void CFlowerLightScript::Fall()
{
	GetOwner()->FlipBookComponent()->ReversePlay(0, 10, false);
}
