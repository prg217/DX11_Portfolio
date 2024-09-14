#include "pch.h"
#include "CJellyPushFrameScript.h"

#include "CJellyPushScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CJellyPushFrameScript::CJellyPushFrameScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHFRAMESCRIPT))
	, m_Type(JellyPushType::NONE)
	, m_StoneBlock(nullptr)
	, m_PuzzleNum(0)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_Type);
	AddScriptParam(SCRIPT_PARAM::INT, "PuzzleNumber", &m_PuzzleNum);
	AddScriptParam(SCRIPT_PARAM::GAMEOBJECT, "StoneBlock", &m_StoneBlock);
}

CJellyPushFrameScript::CJellyPushFrameScript(const CJellyPushFrameScript& _Origin)
	: CScript(_Origin)
	, m_Type(JellyPushType::NONE)
	, m_StoneBlock(nullptr)
	, m_PuzzleNum(0)
{
}

CJellyPushFrameScript::~CJellyPushFrameScript()
{
}

void CJellyPushFrameScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	switch (m_PuzzleNum)
	{
	case 0:
		m_StoneBlock = curLevel->FindObjectByName(L"StoneBlock");
		break;
	default:
		break;
	}

}

void CJellyPushFrameScript::Tick()
{
}

void CJellyPushFrameScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// ���� Ÿ�԰� ���� ������ ���� ����(������ 25��ŭ �̵�)
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
	fwrite(&m_Type, sizeof(JellyPushType), 1, _File);
	fwrite(&m_PuzzleNum, sizeof(int), 1, _File);
	//fwrite(&m_StoneBlock, sizeof(CGameObject*), 1, _File);
}

void CJellyPushFrameScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(JellyPushType), 1, _File);
	fread(&m_PuzzleNum, sizeof(int), 1, _File);
	//fread(&m_StoneBlock, sizeof(CGameObject*), 1, _File);
}
