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
	// 어느 젤리든 본인 위치에 끌어옴
	// 본인 타입과 같은 젤리면 길을 열음
	// 큰 젤리는 한 번 정답 맞추면 움직일 수 없게 아예 삭제하고 모양만 갖춘거 새로 만들까?
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
