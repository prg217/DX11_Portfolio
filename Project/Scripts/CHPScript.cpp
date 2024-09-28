#include "pch.h"
#include "CHPScript.h"

#include "CPlayerScript.h"
#include "CMonsterScript.h"
#include "CBugBossScript.h"

#include <Engine/CFontMgr.h>

CHPScript::CHPScript()
	: CScript(UINT(SCRIPT_TYPE::HPSCRIPT))
	, m_HP(0)
	, m_MaxHP(0)
	, m_HPBar(nullptr)
	, m_Player(false)
	, m_OriginPosition(Vec3(0.f, 0.f, 0.f))
	, m_OriginScale(Vec3(0.f, 0.f, 0.f))
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "MaxHP", &m_MaxHP);
}

CHPScript::CHPScript(const CHPScript& _Origin)
	: CScript(_Origin)
	, m_HP(0)
	, m_MaxHP(0)
	, m_HPBar(nullptr)
	, m_Player(false)
	, m_OriginPosition(Vec3(0.f, 0.f, 0.f))
	, m_OriginScale(Vec3(0.f, 0.f, 0.f))
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
			// HP�� ����
			DeleteObject(m_HPBar);
		}

		monsterScript->Dead();
		return;
	}
	CPlayerScript* playerScript = dynamic_cast<CPlayerScript*>(GetOwner()->GetScript("CPlayerScript"));
	if (playerScript != nullptr)
	{
		// ���� ������ �� �߱� ������ ���� ���� ���� ü���� 1���� �� �� ��������
		m_HP = 1;
		//playerScript->Dead();
		return;
	}
	CBugBossScript* bpssScript = dynamic_cast<CBugBossScript*>(GetOwner()->GetScript("CBugBossScript"));
	if (bpssScript != nullptr)
	{
		if (m_HPBar != nullptr)
		{
			// HP�� ����
			DeleteObject(m_HPBar);
		}

		bpssScript->Dead();
		return;
	}
}

void CHPScript::Hit(int _Damage, CGameObject* _HPBar)
{
	if (_HPBar != nullptr)
	{
		m_HPBar = _HPBar;
	}

	if (GetOwner()->GetLayerIdx() == 3)
	{
		// �÷��̾�� �ǰ� 1 ���Ϸ� �������� �ʰ� ��
		if (m_HP == 1)
		{
			return;
		}
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
		if (m_OriginScale == Vec3(0.f, 0.f, 0.f))
		{
			m_OriginScale = m_HPBar->Transform()->GetRelativeScale();
		}
		float hp = static_cast<float>(m_HP) / static_cast<float>(m_MaxHP);

		if (m_OriginPosition == Vec3(0.f, 0.f, 0.f))
		{
			// HP ���� ���� ��ġ�� ������
			m_OriginPosition = m_HPBar->Transform()->GetRelativePos();
		}

		// HP ���� ���� ũ�⸦ ü�� ������ ���߱�
		float newScaleX = hp * m_OriginScale.x;

		m_HPBar->Transform()->SetRelativeScale(Vec3(newScaleX, m_OriginScale.y, m_OriginScale.z));

		float ratio = 0.4f;
		if (wcscmp(GetOwner()->GetName().c_str(), L"BugBoss") == 0)
		{
			ratio = 0.48f;
		}
		float positionOffset = (1.0f - hp) * ratio;

		// �� ��ġ ���� (���� ����)
		m_HPBar->Transform()->SetRelativePos(Vec3(m_OriginPosition.x - positionOffset, m_OriginPosition.y, m_OriginPosition.z));
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
		if (m_OriginScale == Vec3(0.f, 0.f, 0.f))
		{
			m_OriginScale = m_HPBar->Transform()->GetRelativeScale();
		}
		float hp = static_cast<float>(m_HP) / static_cast<float>(m_MaxHP);

		if (m_OriginPosition == Vec3(0.f, 0.f, 0.f))
		{
			// HP ���� ���� ��ġ�� ������
			m_OriginPosition = m_HPBar->Transform()->GetRelativePos();
		}

		// HP ���� ���� ũ�⸦ ü�� ������ ���߱�
		float newScaleX = hp * m_OriginScale.x;

		m_HPBar->Transform()->SetRelativeScale(Vec3(newScaleX, m_OriginScale.y, m_OriginScale.z));

		float ratio = 0.4f;
		if (wcscmp(GetOwner()->GetName().c_str(), L"BugBoss") == 0)
		{
			ratio = 0.48f;
		}
		float positionOffset = (1.0f - hp) * ratio;

		// �� ��ġ ���� (���� ����)
		m_HPBar->Transform()->SetRelativePos(Vec3(m_OriginPosition.x - positionOffset, m_OriginPosition.y, m_OriginPosition.z));
	}
}
