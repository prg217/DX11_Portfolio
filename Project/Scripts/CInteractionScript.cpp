#include "pch.h"
#include "CInteractionScript.h"

CInteractionScript::CInteractionScript()
	: CScript(UINT(SCRIPT_TYPE::INTERACTIONSCRIPT))
{
}

CInteractionScript::CInteractionScript(const CInteractionScript& _Origin)
	: CScript(_Origin)
{
}

CInteractionScript::~CInteractionScript()
{
}

void CInteractionScript::Begin()
{
}

void CInteractionScript::Tick()
{
}

void CInteractionScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CInteractionScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

}

void CInteractionScript::SaveToFile(FILE* _File)
{
}

void CInteractionScript::LoadFromFile(FILE* _File)
{
}
