#include "pch.h"
#include "CColorBugScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CBugBossScript.h"

CColorBugScript::CColorBugScript()
	: CScript(UINT(SCRIPT_TYPE::COLORBUGSCRIPT))
	, m_Type(ColorBugType::Blue)
	, m_SaveTime(0.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Type", &m_Type);
}

CColorBugScript::CColorBugScript(const CColorBugScript& _Origin)
	: CScript(_Origin)
	, m_Type(_Origin.m_Type)
	, m_SaveTime(0.f)
{
}

CColorBugScript::~CColorBugScript()
{
}

void CColorBugScript::Begin()
{
}

void CColorBugScript::Tick()
{
	// �����¿�, �밢������ �����ϰ� �̵�(z�൵ y�� ���� ����...)
	m_SaveTime += DT;

	if (m_SaveTime >= 3.f)
	{
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();



		m_SaveTime = 0.f;
	}
}

void CColorBugScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CColorBugScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CColorBugScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CColorBugScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(ColorBugType), 1, _File);
}

void CColorBugScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(ColorBugType), 1, _File);
}

void CColorBugScript::Dead()
{
	// �״� ����Ʈ

	// BugBoss���� ��ȣ ����
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* boss = curLevel->FindObjectByName(L"BugBoss");
	CBugBossScript* bossScript = dynamic_cast<CBugBossScript*>(boss->GetScript("CBugBossScript"));
	bossScript->Phase2Down(m_Type);
}
