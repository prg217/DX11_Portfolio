#include "pch.h"
#include "CFlowerLightAreaScript.h"

#include "CJellyPushScript.h"

CFlowerLightAreaScript::CFlowerLightAreaScript()
	: CScript(UINT(SCRIPT_TYPE::FLOWERLIGHTAREASCRIPT))
	, m_Type(JellyPushType::NONE)
{
}

CFlowerLightAreaScript::CFlowerLightAreaScript(const CFlowerLightAreaScript& _Origin)
	: CScript(_Origin)
	, m_Type(_Origin.m_Type)
{
}

CFlowerLightAreaScript::~CFlowerLightAreaScript()
{
}

void CFlowerLightAreaScript::Begin()
{
}

void CFlowerLightAreaScript::Tick()
{
}

void CFlowerLightAreaScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightAreaScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightAreaScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightAreaScript::SaveToFile(FILE* _File)
{
}

void CFlowerLightAreaScript::LoadFromFile(FILE* _File)
{
}

void CFlowerLightAreaScript::SetJellyPushType(JellyPushType _Type)
{
	m_Type = _Type;

	switch (m_Type)
	{
	case JellyPushType::CYAN:
		GetOwner()->SpriteComponent()->AddColor(true, Vec3(0.f, 0.7f, 0.93f));
		break;
	case JellyPushType::MAGENTA:
		GetOwner()->SpriteComponent()->AddColor(true, Vec3(1.f, 0.4f, 0.75f));
		break;
	case JellyPushType::YELLOW:
		GetOwner()->SpriteComponent()->AddColor(true, Vec3(1.f, 0.95f, 0.f));
		break;
	case JellyPushType::BLUE:
		GetOwner()->SpriteComponent()->AddColor(true, Vec3(0.1f, 0.27f, 0.7f));
		break;
	case JellyPushType::GREEN:
		GetOwner()->SpriteComponent()->AddColor(true, Vec3(0.f, 0.65f, 0.16f));
		break;
	case JellyPushType::RED:
		GetOwner()->SpriteComponent()->AddColor(true, Vec3(1.f, 0.25f, 0.25f));
		break;
	default:
		break;
	}
}
