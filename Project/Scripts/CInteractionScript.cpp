#include "pch.h"
#include "CInteractionScript.h"

CInteractionScript::CInteractionScript()
	: CScript(UINT(SCRIPT_TYPE::INTERACTIONSCRIPT))
	, m_OutlineCheck(true)
{
}

CInteractionScript::CInteractionScript(const CInteractionScript& _Origin)
	: CScript(_Origin)
	, m_OutlineCheck(true)
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
	if (!m_OutlineCheck)
	{
		return;
	}

	if (_OtherObject->GetLayerIdx() == 5)
	{
		// ¿Ü°û¼± Àû¿ë
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
		// ¿Ü°û¼± Àû¿ë
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

void CInteractionScript::SetOutlineCheck(bool _Check)
{
	m_OutlineCheck = _Check;

	if (!_Check)
	{
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
