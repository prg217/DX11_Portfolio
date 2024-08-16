#include "pch.h"
#include "ListUI.h"

ListUI::ListUI()
	: m_SelectedIdx(-1)
	, m_UI(nullptr)
	, m_MemFunc(nullptr)
	, m_IntT(0)
{
}

ListUI::~ListUI()	
{
}

void ListUI::Update()
{
	for (size_t i = 0; i < m_vecList.size(); ++i)
	{
		char szID[255] = {};
		sprintf_s(szID, 255, "%s##%d", m_vecList[i].c_str(), (int)i);		

		UINT Flag = ImGuiTreeNodeFlags_Leaf;

		if (i == m_SelectedIdx)
		{
			Flag |= ImGuiTreeNodeFlags_Selected;
		}

		if (ImGui::TreeNodeEx(szID, Flag))
		{
			ImGui::TreePop();
		}
		
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			m_SelectedIdx = (int)i;
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			m_SelectedName = m_vecList[i];

			if (m_UI && m_MemFunc)
			{
				(m_UI->*m_MemFunc)((DWORD_PTR)this);
			}

			if (m_UI && m_MemFunc2)
			{
				// int만 되고 있는데, 다른 자료형도 되도록 나눠야 함
				(m_UI->*m_MemFunc2)((DWORD_PTR)this, (DWORD_PTR)m_IntT);
			}

			SetActive(false);
		}

		/*if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			m_SelectedIdx = i;
		}*/
	}
}

void ListUI::AddItem(const string& _item)
{
	m_vecList.push_back(_item);
}

void ListUI::AddList(const vector<string>& _list)
{
	m_vecList.insert(m_vecList.end(), _list.begin(), _list.end());
}

void ListUI::Activate()
{
	m_vecList.insert(m_vecList.begin(), 1, "None");
}

void ListUI::Deactivate()
{
	// Clear
	m_vecList.clear();
	m_SelectedIdx = -1;
	m_UI = nullptr;
	m_MemFunc = nullptr;
	m_MemFunc2 = nullptr;
}
