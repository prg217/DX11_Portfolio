#include "pch.h"
#include "CJellyPushScript.h"

CJellyPushScript::CJellyPushScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHSCRIPT))
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_Type);
}

CJellyPushScript::CJellyPushScript(const CJellyPushScript& _Origin)
	: CScript(_Origin)
{
}

CJellyPushScript::~CJellyPushScript()
{
}

void CJellyPushScript::Begin()
{
}

void CJellyPushScript::Tick()
{
}

void CJellyPushScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 넣는 구멍에 닿았으면 자동으로 가운데에 안착

	// 미니 젤리 일 경우
	// 같은 색이면 같이 밀려짐
	// 다른 색이면 그 색에 따라 합성됨(서로 위치 가운데 지점으로 가다가 합쳐짐)

	// 큰 젤리 일 경우
	// 들면 분리되어 랜덤 1개를 플레이어가 들게 됨
}

void CJellyPushScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushScript::SaveToFile(FILE* _File)
{
}

void CJellyPushScript::LoadFromFile(FILE* _File)
{
}
