#include "pch.h"
#include "FlipBookComUI.h"

#include <Engine/CFlipBookComponent.h>
#include <Engine/CAssetMgr.h>

#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

FlipBookComUI::FlipBookComUI()
	: ComponentUI(COMPONENT_TYPE::FLIPBOOKCOMPONENT)
{
}

FlipBookComUI::~FlipBookComUI()
{
}


void FlipBookComUI::Update()
{
	Title();

	// AddFlipBook목록, 그리고 추가할 수 있게
	CFlipBookComponent* flipCom = GetTargetObject()->FlipBookComponent();
	for (int i = 0; i < flipCom->GetFlipBooks().size() + 1; i++)
	{
		FlipBook(i);
	}
}

void FlipBookComUI::FlipBook(int _Count)
{
	string FlipBookName;

	CFlipBookComponent* flipCom = GetTargetObject()->FlipBookComponent();
	m_vFlipBook = flipCom->GetFlipBooks();
	if (_Count < m_vFlipBook.size())
	{
		if (m_vFlipBook[_Count] != nullptr)
		{
			FlipBookName = string(m_vFlipBook[_Count]->GetKey().begin(), m_vFlipBook[_Count]->GetKey().end());
		}
	}

	const char* name = "FlipBook";
	string tempCount = std::to_string(_Count);
	const char* nameCount = (char*)tempCount.c_str();

	// 플립북 순서로 ID
	ImGui::Text(name);
	ImGui::SameLine(60);
	ImGui::Text(nameCount);
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(200.f);

	string tempID = "##FlipBook" + tempCount;
	const char* nameID_1 = tempID.c_str();
	ImGui::InputText(nameID_1, (char*)FlipBookName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::FLIPBOOK == pAsset->GetAssetType())
			{
				if (_Count >= m_vFlipBook.size())
				{
					m_vFlipBook.push_back((CFlipBook*)pAsset.Get());
				}
				else
				{
					m_vFlipBook[_Count] = (CFlipBook*)pAsset.Get();
				}

				flipCom->SetFlipBook(m_vFlipBook);
			}
		}

		ImGui::EndDragDropTarget();
	}

	tempID = "##FlipBookBtn" + tempCount;
	const char* nameID_2 = tempID.c_str();
	ImGui::SameLine();
	if (ImGui::Button(nameID_2, ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("FlipBook");
		vector<string> vecFlipBookNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::FLIPBOOK, vecFlipBookNames);
		pListUI->AddList(vecFlipBookNames);
		pListUI->AddDelegate(this, (DELEGATE_2)&FlipBookComUI::SelectFlipBook, _Count);
		pListUI->SetActive(true);
		// 두번째 시도부터 UI가 뒤에 있음
	}
}

void FlipBookComUI::SelectFlipBook(DWORD_PTR _ListUI, DWORD_PTR _Count)
{
	CFlipBookComponent* flipCom = GetTargetObject()->FlipBookComponent();

	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		if (_Count == 0 && m_vFlipBook.size() == 0)
		{
			return;
		}
		if (_Count == m_vFlipBook.size())
		{
			return;
		}

		m_vFlipBook.erase(m_vFlipBook.begin() + _Count);

		flipCom->SetFlipBook(m_vFlipBook);

		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CFlipBook> pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(strAssetName);

	assert(pFlipBook.Get());

	if (_Count >= m_vFlipBook.size())
	{
		m_vFlipBook.push_back(pFlipBook.Get());
	}
	else
	{
		m_vFlipBook[_Count] = pFlipBook.Get();
	}

	flipCom->SetFlipBook(m_vFlipBook);
}
