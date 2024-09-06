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
	, m_NextObj(nullptr)
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
	, m_NextObj(nullptr)
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
			// next ������
			Ptr<CFlipBook> pFlipBook;
			Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");

			m_NextObj = new CGameObject;
			m_NextObj->SetName(L"TextBoxNext");
			m_NextObj->AddComponent(new CTransform);
			m_NextObj->AddComponent(new CMeshRender);
			m_NextObj->AddComponent(new CFlipBookComponent);

			m_NextObj->Transform()->SetRelativePos(0.f, -0.4f, 0.f);
			m_NextObj->Transform()->SetRelativeScale(0.3f, 0.3f, 0.f);

			m_NextObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			m_NextObj->MeshRender()->SetMaterial(pMtrl);

			pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\UI\\textBox_next.flip");
			m_NextObj->FlipBookComponent()->AddFlipBook(0, pFlipBook);
			m_NextObj->FlipBookComponent()->Play(0, 3, true);

			CreateObject(m_NextObj, 31);
			AddChildObject(GetOwner(), m_NextObj);
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
			// �ٹٲ޸��� y���� �ٲ��ش�.
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
			if (KEY_TAP(KEY::S) || KEY_TAP(KEY::A)) // �Ǵ� ���콺 Ŭ��
			{
				m_TextIdx++;
				m_TextCount = 1;
				m_TextPosY = 600.f;
				DeleteObject(m_NextObj);
				m_NextObj = nullptr;

				if (m_TextIdx >= m_vText.size())
				{
					m_TextIdx = -1;
					m_TextCount = 1;
					// ��ȭâ ����
					for (int i = 0; i < GetOwner()->GetChildren().size(); i++)
					{
						DeleteObject(GetOwner()->GetChildren()[i]);
					}
					DeleteObject(GetOwner());
				}
			}
		}

		// ��ȭ�� �� ���� ���� �Ѵ�.
		if ((KEY_TAP(KEY::S) || KEY_TAP(KEY::A)) && m_TextIdx != -1) // �Ǵ� ���콺 Ŭ��
		{
			m_TextCount = m_vText[m_TextIdx].length();
			m_TextPosY = 600.f - (30.f - m_TextCount);
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

