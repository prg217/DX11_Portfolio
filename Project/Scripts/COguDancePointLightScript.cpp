#include "pch.h"
#include "COguDancePointLightScript.h"

COguDancePointLightScript::COguDancePointLightScript()
	: CScript(UINT(SCRIPT_TYPE::OGUDANCEPOINTLIGHTSCRIPT))
	, m_MaxRadius(80.f)
	, m_MinRadius(60.f)
	, m_Duration(1.f)
	, m_Time(0.f)
{
}

COguDancePointLightScript::COguDancePointLightScript(const COguDancePointLightScript& _Origin)
	: CScript(_Origin)
	, m_MaxRadius(80.f)
	, m_MinRadius(60.f)
	, m_Duration(1.f)
	, m_Time(0.f)
{
}

COguDancePointLightScript::~COguDancePointLightScript()
{
}

void COguDancePointLightScript::Begin()
{
}

void COguDancePointLightScript::Tick()
{
	if (GetOwner()->Light2D() == nullptr)
	{
		return;
	}

	m_Time += DT;

	if (m_Time <= m_Duration)
	{
		float value = m_MaxRadius - ((m_Time / m_Duration) * (m_MaxRadius - m_MinRadius));
		GetOwner()->Light2D()->SetRadius(value);
	}
}

void COguDancePointLightScript::SaveToFile(FILE* _File)
{
}

void COguDancePointLightScript::LoadFromFile(FILE* _File)
{
}