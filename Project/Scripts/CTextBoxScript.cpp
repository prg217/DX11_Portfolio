#include "pch.h"
#include "CTextBoxScript.h"

#include <Engine/CFontMgr.h>

CTextBoxScript::CTextBoxScript()
	: CScript(UINT(SCRIPT_TYPE::TEXTBOXSCRIPT))
	, m_IsName(false)
	, m_SaveTime(0.f)
	, m_NextTime(0.1f)
	, m_TextCount(1)
	, m_TextPosY(600.f)
	, m_TextIdx(0)
{
}

CTextBoxScript::CTextBoxScript(const CTextBoxScript& _Origin)
	: CScript(_Origin)
	, m_IsName(_Origin.m_IsName)
	, m_SaveTime(0.f)
	, m_NextTime(_Origin.m_NextTime)
	, m_TextCount(1)
	, m_TextPosY(600.f)
	, m_TextIdx(0)
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
		wstring text = m_vText[m_TextIdx].substr(0, m_TextCount);
		if (TIME - m_SaveTime >= m_NextTime)
		{
			// 줄바꿈마다 y축을 바꿔준다.
			if (text.back() == L'\n')
			{
				m_TextPosY -= 30.f;
			}

			if (m_TextCount >= m_vText[m_TextIdx].length())
			{
				if (KEY_PRESSED(KEY::S) || KEY_PRESSED(KEY::A)) // 또는 마우스 클릭
				{
					m_TextIdx++;
					m_TextCount = 1;
					m_TextPosY = 600.f;
					
					if (m_TextIdx >= m_vText.size())
					{
						m_TextIdx = 0;
						m_TextCount = 1;
						// 대화창 삭제
						DeleteObject(GetOwner()->GetChildren()[0]);
						DeleteObject(GetOwner());
					}
				}
			}
			else
			{
				m_TextCount++;
			}
			m_SaveTime = TIME;
		}

		CFontMgr::GetInst()->DrawCenterFont(text.c_str(), 650, m_TextPosY, 30, FONT_RGBA(236, 230, 206, 255));
	}
}

void CTextBoxScript::SaveToFile(FILE* _File)
{
	//m_IsName
}

void CTextBoxScript::LoadFromFile(FILE* _File)
{
}

void CTextBoxScript::SetText(wstring _Text)
{
	m_vText.push_back(_Text);
}

