#include "pch.h"
#include "CSpitCactusScript.h"

CSpitCactusScript::CSpitCactusScript()
	: CScript(UINT(SCRIPT_TYPE::SPITCACTUSSCRIPT))
	, m_Attack(false)
	, m_Dead(false)
	, m_SaveAttackTime(0)
	, m_AttackTime(2.f)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Needle0", &m_NeedleObj0);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Needle1", &m_NeedleObj1);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Needle2", &m_NeedleObj2);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Needle3", &m_NeedleObj3);
}

CSpitCactusScript::CSpitCactusScript(const CSpitCactusScript& _Origin)
	: CScript(_Origin)
	, m_Attack(false)
	, m_Dead(false)
	, m_SaveAttackTime(0)
	, m_AttackTime(2.f)
{
}

CSpitCactusScript::~CSpitCactusScript()
{
}

void CSpitCactusScript::Begin()
{
}

void CSpitCactusScript::Tick()
{
	if (m_Attack)
	{
		// Ư�� �ð� ���� ���� ����
		if (TIME - m_SaveAttackTime >= m_AttackTime)
		{
			m_SaveAttackTime = TIME;
			SpawnNeedle();
		}
	}
}

void CSpitCactusScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSpitCactusScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSpitCactusScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSpitCactusScript::SaveToFile(FILE* _File)
{

}

void CSpitCactusScript::LoadFromFile(FILE* _File)
{

}

void CSpitCactusScript::SpawnNeedle()
{
	// �����¿� ������ ���� �߻�
	if (m_NeedleObj0 != nullptr)
	{
		Instantiate(m_NeedleObj0, 10, Transform()->GetWorldPos(), L"CactusNeedle");
	}
	if (m_NeedleObj1 != nullptr)
	{
		Instantiate(m_NeedleObj1, 10, Transform()->GetWorldPos(), L"CactusNeedle");
	}
	if (m_NeedleObj2 != nullptr)
	{
		Instantiate(m_NeedleObj2, 10, Transform()->GetWorldPos(), L"CactusNeedle");
	}
	if (m_NeedleObj3 != nullptr)
	{
		Instantiate(m_NeedleObj3, 10, Transform()->GetWorldPos(), L"CactusNeedle");
	}
}

void CSpitCactusScript::Attack()
{
	// �ִϸ��̼� Ȱ��ȭ
	m_Attack = true;
	m_SaveAttackTime = TIME;
	GetOwner()->FlipBookComponent()->Play(0, 8, true);
}

void CSpitCactusScript::Stop()
{
	// ���� ����
	m_Attack = false;
	// �ִϸ��̼� ��Ȱ��ȭ
	GetOwner()->FlipBookComponent()->Play(0, 0, false);
}

void CSpitCactusScript::Dead()
{
	// ���� ���߰� ���� �ִϸ��̼�
	m_Attack = false;
	m_Dead = true;
	GetOwner()->FlipBookComponent()->Play(1, 8, false);
	// ���� �ִϸ��̼� �� ��� �� ����
}
