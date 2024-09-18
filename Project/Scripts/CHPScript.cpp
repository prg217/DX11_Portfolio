#include "pch.h"
#include "CHPScript.h"

CHPScript::CHPScript()
	: CScript(UINT(SCRIPT_TYPE::HPSCRIPT))
{
	AddScriptParam(SCRIPT_PARAM::INT, "HP", &m_HP);
	AddScriptParam(SCRIPT_PARAM::INT, "MaxHP", &m_MaxHP);
}

CHPScript::CHPScript(const CHPScript& _Origin)
	: CScript(_Origin)
{
}

CHPScript::~CHPScript()
{
}

void CHPScript::Begin()
{
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
}

void CHPScript::Hit(int _Damage)
{
	m_HP -= _Damage;

	if (m_HP <= 0)
	{
		m_HP = 0;
		Dead();
	}
}

void CHPScript::Heal(int _Heal)
{
	m_HP += _Heal;

	if (m_MaxHP <= m_HP)
	{
		m_HP = m_MaxHP;
	}
}
