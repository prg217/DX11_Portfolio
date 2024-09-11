#include "pch.h"
#include "CCountDownDeleteScript.h"

CCountDownDeleteScript::CCountDownDeleteScript()
	: CScript(UINT(SCRIPT_TYPE::COUNTDOWNDELETESCRIPT))
	, m_SaveTime(TIME)
	, m_DeadTime(5.f)
{
}

CCountDownDeleteScript::CCountDownDeleteScript(const CCountDownDeleteScript& _Origin)
	: CScript(_Origin)
	, m_SaveTime(TIME)
	, m_DeadTime(5.f)
{
}

CCountDownDeleteScript::~CCountDownDeleteScript()
{
}


void CCountDownDeleteScript::Begin()
{
	//m_SaveTime = TIME;
}

void CCountDownDeleteScript::Tick()
{
	if (TIME - m_SaveTime >= m_DeadTime)
	{
		DeleteObject(GetOwner());
	}
}

void CCountDownDeleteScript::SaveToFile(FILE* _File)
{
}

void CCountDownDeleteScript::LoadFromFile(FILE* _File)
{
}