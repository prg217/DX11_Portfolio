#include "pch.h"
#include "CTextBoxScript.h"

#include <Engine/CFontMgr.h>

CTextBoxScript::CTextBoxScript()
	: CScript(UINT(SCRIPT_TYPE::TEXTBOXSCRIPT))
	, m_IsName(false)
	, m_SaveTime(0.f)
	, m_NextTime(0.2f)
	, m_TextCount(1)
{
}

CTextBoxScript::CTextBoxScript(const CTextBoxScript& _Origin)
	: CScript(_Origin)
	, m_IsName(_Origin.m_IsName)
	, m_SaveTime(0.f)
	, m_NextTime(_Origin.m_NextTime)
	, m_TextCount(1)
{
}

CTextBoxScript::~CTextBoxScript()
{
}

void CTextBoxScript::Begin()
{
	m_SaveTime = TIME;
}

void CTextBoxScript::Tick()
{


}

void CTextBoxScript::Render()
{
	if (m_vText.size() > 0)
	{
		wstring text = m_vText[0].substr(0, m_TextCount);
		if (TIME - m_SaveTime >= m_NextTime)
		{
			m_TextCount++;
			if (m_TextCount > m_vText[0].length())
			{
				m_TextCount = 1;
			}
			m_SaveTime = TIME;
		}
		float posX = 500;
		float posY = 100 - ((m_TextLine[0] * 25) / 2);
		CFontMgr::GetInst()->DrawCenterFont(text.c_str(), posX, posY, 25, FONT_RGBA(255, 255, 255, 255));
	}
}

void CTextBoxScript::SaveToFile(FILE* _File)
{
	//m_IsName
}

void CTextBoxScript::LoadFromFile(FILE* _File)
{
}

void CTextBoxScript::SetText(int _Idx, int _Line, wstring _Text)
{
	m_TextIdx.push_back(_Idx);
	m_TextLine.push_back(_Line);
	m_vText.push_back(_Text);
}

