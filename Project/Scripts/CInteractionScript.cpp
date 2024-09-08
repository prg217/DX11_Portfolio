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
	if (_OtherObject->GetLayerIdx() == 5)
	{
		// �ܰ��� ����
		if (GetOwner()->GetComponent(COMPONENT_TYPE::SPRITECOMPONENT) != nullptr)
		{
			GetOwner()->SpriteComponent()->SetOutline(true);
		}
		else if (GetOwner()->GetComponent(COMPONENT_TYPE::FLIPBOOKCOMPONENT) != nullptr)
		{
			GetOwner()->FlipBookComponent()->SetOutline(true);
		}
	}
}

void CInteractionScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 5)
	{
		// �ܰ��� ����
		if (GetOwner()->GetComponent(COMPONENT_TYPE::SPRITECOMPONENT) != nullptr)
		{
			GetOwner()->SpriteComponent()->SetOutline(false);
		}
		else if (GetOwner()->GetComponent(COMPONENT_TYPE::FLIPBOOKCOMPONENT) != nullptr)
		{
			GetOwner()->FlipBookComponent()->SetOutline(false);
		}
	}
}

void CInteractionScript::SaveToFile(FILE* _File)
{
}

void CInteractionScript::LoadFromFile(FILE* _File)
{
}
