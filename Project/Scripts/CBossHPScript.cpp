#include "pch.h"
#include "CBossHPScript.h"

#include <Engine/CFontMgr.h>

CBossHPScript::CBossHPScript()
	: CScript(UINT(SCRIPT_TYPE::BOSSHPSCRIPT))
	, m_X(0)
	, m_Y(0)
	, m_Name(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "X", &m_X);
	AddScriptParam(SCRIPT_PARAM::INT, "Y", &m_Y);
}

CBossHPScript::CBossHPScript(const CBossHPScript& _Origin)
	: CScript(_Origin)
	, m_X(0)
	, m_Y(0)
	, m_Name(false)
{
}

CBossHPScript::~CBossHPScript()
{
}

void CBossHPScript::Begin()
{
}

void CBossHPScript::Tick()
{
}

void CBossHPScript::Render()
{
	if (!m_Name)
	{
		return;
	}

	wstring text = L"타락한 누에 용사";
	wstring fontName = L"카페24 써라운드";

	// 글씨
	CFontMgr::GetInst()->DrawCenterFont(text.c_str(), m_X, m_Y, 40, FONT_RGBA(236, 230, 206, 255), fontName);
}

void CBossHPScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossHPScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossHPScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossHPScript::SaveToFile(FILE* _File)
{
	fwrite(&m_X, sizeof(int), 1, _File);
	fwrite(&m_Y, sizeof(int), 1, _File);
}

void CBossHPScript::LoadFromFile(FILE* _File)
{
	fread(&m_X, sizeof(int), 1, _File);
	fread(&m_Y, sizeof(int), 1, _File);
}

void CBossHPScript::Start()
{
	GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 290.f, -5000.f));

	m_Name = true;
}
