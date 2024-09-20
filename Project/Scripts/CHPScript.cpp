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
	m_HP = m_MaxHP;
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

void CHPScript::Hit(int _Damage, CGameObject* _HPBar)
{
	// 플레이어나 보스가 아닐 경우 HP바 활성화 시켜야 함

	if (_HPBar != nullptr)
	{
		m_HPBar = _HPBar;
	}

	m_HP -= _Damage;

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

	if (m_HP <= 0)
	{
		// HP바 사라지기
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
