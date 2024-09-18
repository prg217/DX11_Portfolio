#include "pch.h"
#include "CBugNPCScript.h"

CBugNPCScript::CBugNPCScript()
	: CScript(UINT(SCRIPT_TYPE::BUGNPCSCRIPT))
{
}

CBugNPCScript::CBugNPCScript(const CBugNPCScript& _Origin)
	: CScript(_Origin)
{
}

CBugNPCScript::~CBugNPCScript()
{
}

void CBugNPCScript::Begin()
{
}

void CBugNPCScript::Tick()
{
}

void CBugNPCScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugNPCScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugNPCScript::SaveToFile(FILE* _File)
{
}

void CBugNPCScript::LoadFromFile(FILE* _File)
{
}
