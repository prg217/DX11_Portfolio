#include "pch.h"
#include "AnimationEditor.h"

#include "CEditorMgr.h"
#include "AE_Detail.h"
#include "AE_SpriteView.h"

#include "ListUI.h"
#include "TreeUI.h"

AnimationEditor::AnimationEditor()
	: m_SpriteView(nullptr)
	, m_Detail(nullptr)
{
	UseMenuBar(true);
}

AnimationEditor::~AnimationEditor()
{
}

void AnimationEditor::Init()
{
	m_SpriteView = (AE_SpriteView*)CEditorMgr::GetInst()->FindEditorUI("AE_SpriteView");
	m_Detail = (AE_Detail*)CEditorMgr::GetInst()->FindEditorUI("AE_Detail");

	m_SpriteView->SetMove(true);
	m_Detail->SetMove(true);

	m_SpriteView->m_Owner = this;
	m_Detail->m_Owner = this;
}

void AnimationEditor::Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool SpriteView = m_SpriteView->IsActive();
			bool Detail = m_Detail->IsActive();

			if (ImGui::MenuItem("SpriteView", nullptr, &SpriteView))
			{
				m_SpriteView->SetActive(SpriteView);
			}

			if (ImGui::MenuItem("Detail", nullptr, &Detail))
			{
				m_Detail->SetActive(Detail);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	for (int i = 0; i < m_vecSprite.size() + 1; i++)
	{
		Sprite(i);
	}
}

void AnimationEditor::Activate()
{
	m_SpriteView->SetActive(true);
	m_Detail->SetActive(true);
}

void AnimationEditor::Deactivate()
{
	m_SpriteView->SetActive(false);
	m_Detail->SetActive(false);
}

void AnimationEditor::Sprite(int _Count)
{
	string SpriteName;

	if (_Count < m_vecSprite.size())
	{
		if (m_vecSprite[_Count] != nullptr)
		{
			SpriteName = string(m_vecSprite[_Count]->GetKey().begin(), m_vecSprite[_Count]->GetKey().end());
		}
	}

	const char* name = "Sprite";
	string tempCount = std::to_string(_Count);
	const char* nameCount = (char*)tempCount.c_str();

	ImGui::Text(name);
	ImGui::SameLine(60);
	ImGui::Text(nameCount);
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150.f);

	string tempID = "##Sprite" + tempCount;
	const char* nameID_1 = tempID.c_str();
	ImGui::InputText(nameID_1, (char*)SpriteName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
			{
				SetSprite((CSprite*)pAsset.Get(), _Count);
			}
		}

		ImGui::EndDragDropTarget();
	}

	tempID = "##SpriteBtn" + tempCount;
	const char* nameID_2 = tempID.c_str();
	ImGui::SameLine();
	if (ImGui::Button(nameID_2, ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Sprite");
		vector<string> vecSpriteNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
		pListUI->AddList(vecSpriteNames);
		pListUI->AddDelegate(this, (DELEGATE_2)&AnimationEditor::SelectSprite, _Count);
		pListUI->SetActive(true);
		// 두번째 시도부터 UI가 뒤에 있음 수정 예정
	}
}

void AnimationEditor::SetSprite(Ptr<CSprite> _Sprite, int _Count)
{
	if (_Count >= m_vecSprite.size())
	{
		m_vecSprite.push_back(_Sprite);
	}
	else
	{
		m_vecSprite[_Count] = _Sprite;
	}
	
	GetSpriteView()->SetSprites(m_vecSprite);
	GetDetail()->SetSprites(m_vecSprite);
}

void AnimationEditor::SelectSprite(DWORD_PTR _ListUI, DWORD_PTR _Count)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		if (_Count == 0 && m_vecSprite.size() == 0)
		{
			return;
		}

		// 맨 뒤의 경우 아예 없애버림
		if (_Count == m_vecSprite.size() - 1 && _Count != 0)
		{
			m_vecSprite.pop_back();
		}
		else if (_Count < m_vecSprite.size())
		{
			m_vecSprite[_Count] = nullptr;
		}

		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(strAssetName);

	assert(pSprite.Get());

	SetSprite(pSprite, _Count);
}
