#include "pch.h"
#include "CJellyPushFrameScript.h"

#include "CJellyPushScript.h"

CJellyPushFrameScript::CJellyPushFrameScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHFRAMESCRIPT))
	, m_Type(JellyPushType::NONE)
{
}

CJellyPushFrameScript::CJellyPushFrameScript(const CJellyPushFrameScript& _Origin)
	: CScript(_Origin)
	, m_Type(JellyPushType::NONE)
{
}

CJellyPushFrameScript::~CJellyPushFrameScript()
{
}

void CJellyPushFrameScript::Begin()
{
}

void CJellyPushFrameScript::Tick()
{
}

void CJellyPushFrameScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// ��� ������ ���� ��ġ�� �����
	// ���� Ÿ�԰� ���� ������ ���� ����
	// ū ������ �� �� ���� ���߸� ������ �� ���� �ƿ� �����ϰ� ��縸 ����� ���� �����?
}

void CJellyPushFrameScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushFrameScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushFrameScript::SaveToFile(FILE* _File)
{
}

void CJellyPushFrameScript::LoadFromFile(FILE* _File)
{
}
