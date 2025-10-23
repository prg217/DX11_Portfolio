#include "pch.h"
#include "CTextBoxScript.h"

#include <Engine/CFontMgr.h>

CTextBoxScript::CTextBoxScript()
	: CScript(UINT(SCRIPT_TYPE::TEXTBOXSCRIPT))
	, m_IsName(false)
	, m_SaveTime(0.f)
	, m_NextTime(0.1f)
	, m_textSize(0)
	, m_TextCount(1)
	, m_TextPosY(600.f)
	, m_TextIdx(0)
	, m_NextObj(nullptr)
	, m_IsNext(false)
	, m_IsSkip(false)
{
}

CTextBoxScript::CTextBoxScript(const CTextBoxScript& _Origin)
	: CScript(_Origin)
	, m_IsName(_Origin.m_IsName)
	, m_SaveTime(0.f)
	, m_NextTime(_Origin.m_NextTime)
	, m_textSize(0)
	, m_TextCount(1)
	, m_TextPosY(600.f)
	, m_TextIdx(0)
	, m_NextObj(nullptr)
	, m_IsNext(false)
	, m_IsSkip(false)
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
	if (m_TextIdx == -1)
	{
		return;
	}

	if (m_TextCount >= m_vText[m_TextIdx].length())
	{
		if (m_NextObj == nullptr)
		{
			// next 아이콘
			Ptr<CFlipBook> pFlipBook;
			Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");

			m_NextObj = new CGameObject;
			m_NextObj->SetName(L"TextBoxNext");
			m_NextObj->AddComponent(new CTransform);
			m_NextObj->AddComponent(new CMeshRender);
			m_NextObj->AddComponent(new CFlipBookComponent);

			m_NextObj->Transform()->SetRelativePos(0.f, -330.f, -100.f);
			m_NextObj->Transform()->SetRelativeScale(50.f, 50.f, 0.f);

			m_NextObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			m_NextObj->MeshRender()->SetMaterial(pMtrl);

			pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\UI\\textBox_next.flip");
			m_NextObj->FlipBookComponent()->AddFlipBook(0, pFlipBook);
			m_NextObj->FlipBookComponent()->Play(0, 8, true);

			CreateObject(m_NextObj, 31);
		}
	}

}

void CTextBoxScript::Render()
{
	if (m_TextIdx == -1)
	{
		return;
	}

	if (m_vText.size() > 0)
	{
		if (m_IsName)
		{
			CFontMgr::GetInst()->DrawCenterFont(m_vText[m_TextIdx].c_str(), 325, 485, 30, FONT_RGBA(236, 230, 206, 255));
			return;
		}

		wstring text = m_vText[m_TextIdx].substr(0, m_TextCount);
		if (TIME - m_SaveTime >= m_NextTime)
		{
			// 줄바꿈마다 y축을 바꿔준다.
			if (text.back() == L'\n')
			{
				m_TextPosY -= 30.f;
			}

			if (m_TextCount < m_vText[m_TextIdx].length())
			{
				m_TextCount++;
			}

			m_SaveTime = TIME;
		}

		if (m_TextCount >= m_vText[m_TextIdx].length())
		{
			if ((KEY_TAP(KEY::S) || KEY_TAP(KEY::A)) && !m_IsSkip) // 또는 마우스 클릭
			{
				m_TextIdx++;
				m_TextCount = 1;
				m_TextPosY = 600.f;
				if (m_NextObj != nullptr)
				{
					DeleteObject(m_NextObj);
				}
				m_NextObj = nullptr;

				m_IsNext = true;

				if (m_TextIdx >= m_vText.size())
				{
					m_TextIdx = -1;
					m_TextCount = 1;
					// 대화창 삭제
					for (int i = 0; i < GetOwner()->GetChildren().size(); i++)
					{
						DeleteObject(GetOwner()->GetChildren()[i]);
					}
					DeleteObject(GetOwner());
				}
			}
		}

		m_IsSkip = false;
		// 대화를 한 번에 보게 한다.
		if ((KEY_TAP(KEY::S) || KEY_TAP(KEY::A)) && m_TextIdx != -1 && !m_IsNext) // 또는 마우스 클릭
		{
			m_TextCount = m_vText[m_TextIdx].length();
			int lineCount = std::count(m_vText[m_TextIdx].begin(), m_vText[m_TextIdx].end(), L'\n');
			m_TextPosY = 600.f - (30.f * lineCount);
			m_IsSkip = true;
		}
		m_IsNext = false;

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

void CTextBoxScript::LoadText(const wstring& _FileName)
{
	// 임시로 받을 string 텍스트
	vector<string> stringText;

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"textBox\\";
	strInitPath += _FileName;
	strInitPath += L".textBox";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	if (File == nullptr)
	{
		return;
	}

	fread(&m_textSize, sizeof(int), 1, File);
	stringText.clear();
	stringText.reserve(m_textSize);

	for (int i = 0; i < m_textSize; ++i)
	{
		size_t strLen = 0;
		fread(&strLen, sizeof(size_t), 1, File);

		if (strLen > 0)
		{
			vector<char> buffer(strLen + 1, 0);
			fread(buffer.data(), sizeof(char), strLen, File);
			stringText.push_back(string(buffer.data()));
		}
		// 내용이 비었으면
		else
		{
			stringText.push_back("");
		}

		fclose(File);

		// 임시로 받은 string텍스트 wstring변환
		if (stringText.empty())
		{
			return;
		}

		m_vText.clear();
		m_vText.reserve(m_textSize);

		for (int i = 0; i < m_textSize; ++i)
		{
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, stringText[i].c_str(), (int)stringText[i].size(), NULL, 0);
			std::wstring wstr(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, stringText[i].c_str(), (int)stringText[i].size(), &wstr[0], size_needed);

			m_vText.push_back(wstr);
		}
	}
}

