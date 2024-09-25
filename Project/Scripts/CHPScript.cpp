#include "pch.h"
#include "CHPScript.h"

#include "CPlayerScript.h"
#include "CMonsterScript.h"

#include <Engine/CFontMgr.h>

CHPScript::CHPScript()
	: CScript(UINT(SCRIPT_TYPE::HPSCRIPT))
	, m_HPBar(nullptr)
	, m_Player(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "MaxHP", &m_MaxHP);
}

CHPScript::CHPScript(const CHPScript& _Origin)
	: CScript(_Origin)
	, m_HPBar(nullptr)
	, m_Player(false)
{
}

CHPScript::~CHPScript()
{
}

void CHPScript::Begin()
{
	m_HP = m_MaxHP;

	CPlayerScript* playerScript = dynamic_cast<CPlayerScript*>(GetOwner()->GetScript("CPlayerScript"));
	if (playerScript != nullptr)
	{
		m_Player = true;
	}
}

void CHPScript::Tick()
{
}

void CHPScript::SaveToFile(FILE* _File)
{
	fwrite(&m_HP, sizeof(int), 1, _File);
	fwrite(&m_MaxHP, sizeof(int), 1, _File);
}

void CHPScript::LoadFromFile(FILE* _File)
{
	fread(&m_HP, sizeof(int), 1, _File);
	fread(&m_MaxHP, sizeof(int), 1, _File);
}

void CHPScript::Dead()
{
	CMonsterScript* monsterScript = dynamic_cast<CMonsterScript*>(GetOwner()->GetScript("CMonsterScript"));
	if (monsterScript != nullptr)
	{
		if (m_HPBar != nullptr)
		{
			// HP바 삭제
			DeleteObject(m_HPBar);
		}

		monsterScript->Dead();
		return;
	}
	CPlayerScript* playerScript = dynamic_cast<CPlayerScript*>(GetOwner()->GetScript("CPlayerScript"));
	if (playerScript != nullptr)
	{
		// 죽음 구현을 안 했기 때문에 이지 모드와 같이 체력을 1에서 더 안 내려가게
		m_HP = 1;
		//playerScript->Dead();
		return;
	}
}

void CHPScript::Hit(int _Damage, CGameObject* _HPBar)
{
	if (_HPBar != nullptr)
	{
		m_HPBar = _HPBar;
	}

	m_HP -= _Damage;

	if (m_HP <= 0)
	{
		m_HP = 0;
		Dead();
		return;
	}

	if (m_HPBar != nullptr)
	{
		Vec3 scale = m_HPBar->Transform()->GetRelativeScale();
		float hp = ((float)m_HP / (float)m_MaxHP);
		m_HPBar->Transform()->SetRelativeScale(Vec3(hp, scale.y, scale.z));

		// HP 바의 원래 위치를 가져옴
		Vec3 originalPosition = m_HPBar->Transform()->GetRelativePos();

		// HP 바의 왼쪽 끝이 고정되도록 위치를 조정
		float offsetX = (scale.x - hp) * 0.4f; // 원래 크기에서 줄어든 크기만큼 왼쪽으로 이동
		m_HPBar->Transform()->SetRelativePos(Vec3(originalPosition.x - offsetX, originalPosition.y, originalPosition.z));
	}
}

void CHPScript::Heal(int _Heal)
{
	m_HP += _Heal;

	if (m_MaxHP <= m_HP)
	{
		m_HP = m_MaxHP;
	}

	if (m_HPBar != nullptr)
	{
		Vec3 scale = m_HPBar->Transform()->GetRelativeScale();
		float hp = ((float)m_HP / (float)m_MaxHP);
		m_HPBar->Transform()->SetRelativeScale(Vec3(hp, scale.y, scale.z));

		// HP 바의 원래 위치를 가져옴
		Vec3 originalPosition = m_HPBar->Transform()->GetRelativePos();

		// HP 바의 왼쪽 끝이 고정되도록 위치를 조정
		float offsetX = (scale.x - hp) * 0.4f; // 원래 크기에서 줄어든 크기만큼 왼쪽으로 이동
		m_HPBar->Transform()->SetRelativePos(Vec3(originalPosition.x - offsetX, originalPosition.y, originalPosition.z));
	}
}
