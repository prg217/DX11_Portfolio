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
		// 특정 시간 마다 가시 스폰
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
	// 상하좌우 프리팹 가시 발사
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
	// 애니메이션 활성화
	m_Attack = true;
	m_SaveAttackTime = TIME;
	GetOwner()->FlipBookComponent()->Play(0, 8, true);
}

void CSpitCactusScript::Stop()
{
	// 공격 멈춤
	m_Attack = false;
	// 애니메이션 비활성화
	GetOwner()->FlipBookComponent()->Play(0, 0, false);
}

void CSpitCactusScript::Dead()
{
	// 공격 멈추고 죽음 애니메이션
	m_Attack = false;
	m_Dead = true;
	GetOwner()->FlipBookComponent()->Play(1, 8, false);
	// 죽음 애니메이션 다 재생 후 삭제
}
