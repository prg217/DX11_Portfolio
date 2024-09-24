#include "pch.h"
#include "Outliner.h"

#include "TreeUI.h"
#include "Inspector.h"
#include "CEditorMgr.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CPathMgr.h>


Outliner::Outliner()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	AddChild(m_Tree);

	// Ʈ�� �ɼ� ����
	// ��Ʈ ������ �ʱ�
	m_Tree->ShowRoot(false); 

	// Drag, Drop On
	m_Tree->UseDrag(true);	
	m_Tree->UseDrop(true);
	
	// Clicked Delegate ���
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Outliner::GameObjectClicked);

	// Self DragDrop Delegate ���
	m_Tree->AddDragDropDelegate(this, (DELEGATE_2)&Outliner::GameObjectAddChild);

	// �ܺ� ��� Delegate ���
	m_Tree->AddDropDelegate(this, (DELEGATE_2)&Outliner::DroppedFromOuter);
	m_Tree->SetDropPayLoadName("ContentTree");
	
	// �˾� �޴�
	m_Tree->AddPopupDelegate(this, (DELEGATE_1)&Outliner::PopupMenu);

	// Asset ���¸� Content �� TreeUI �� �ݿ�
	RenewLevel();
}

Outliner::~Outliner()
{
}

void Outliner::Update()
{
	if (CLevelMgr::GetInst()->IsLevelChanged())
		RenewLevel();
}

void Outliner::RenewLevel()
{
	// ��� ���� ����
	m_Tree->Clear();

	// ��Ʈ��� ����
	TreeNode* pRootNode = m_Tree->AddNode(nullptr, "Root", 0);

	
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pLevel)
		return;
	
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pLevel->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecObjects.size(); ++i)
		{
			if (vecObjects[i]->IsDead())
			{
				continue;
			}

			AddGameObject(pRootNode, vecObjects[i]);
		}
	}	
}

void Outliner::AddGameObject(TreeNode* pNode, CGameObject* _Object)
{
	string ObjectName = string(_Object->GetName().begin(), _Object->GetName().end());

	TreeNode* pObjectNode = m_Tree->AddNode(pNode, ObjectName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChildren();

	for (size_t i = 0; i < vecChild.size(); ++ i)
	{
		AddGameObject(pObjectNode, vecChild[i]);
	}
}

void Outliner::GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2)
{
	TreeNode* pDragNode = (TreeNode*)_Param1;
	TreeNode* pDropNode = (TreeNode*)_Param2;

	CGameObject* pDragObject = (CGameObject*)pDragNode->GetData();
	CGameObject* pDropObject = nullptr;

	// Drag ������Ʈ�� Drop ������Ʈ�� �ڽ����� �־��ش�.
	if (pDropNode)
	{
		pDropObject = (CGameObject*)pDropNode->GetData();

		// �ڽ����� �� ������Ʈ�� �θ�(����) �� �ϳ����ٸ� �����Ѵ�.
		if (pDropObject->IsAncestor(pDragObject))
			return;

		pDropObject->AddChild(pDragObject);
	}

	// Drop �������� ���� ������, Drag ������Ʈ�� �ֻ��� �θ�� ������ش�.
	else
	{
		if (!pDragObject->GetParent())
			return;

		// �θ������Ʈ�� ������ ����
		pDragObject->DeregisterChild();

		// ���� �Ҽ� ���̾ �ֻ��� �θ�μ� ���� �Ѵ�.
		CLevelMgr::GetInst()->GetCurrentLevel()->RegisterAsParent(pDragObject->GetLayerIdx(), pDragObject);
	}
}

void Outliner::DroppedFromOuter(DWORD_PTR _OuterData, DWORD_PTR _DropNode)
{
	TreeNode* ContentNode = *((TreeNode**)_OuterData);
	TreeNode* DropNode = (TreeNode*)_DropNode;

}

void Outliner::GameObjectClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;

	CGameObject* pObject = (CGameObject*)pNode->GetData();

	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetObject(pObject);
	//ImGui::SetWindowFocus(nullptr);
}

void Outliner::PopupMenu(DWORD_PTR _Param)
{
	if (ImGui::Button("Add Prefab"))
	{
		TreeNode* pNode = (TreeNode*)_Param;
		CGameObject* pObject = (CGameObject*)pNode->GetData();

		Ptr<CPrefab> pPrefab = new CPrefab;
		pPrefab->SetProtoObject(pObject);

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"prefab\0*.pref";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// Ž��â �ʱ� ��ġ ����
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"prefab\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			pPrefab->Save(szSelect);
		}

		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Delete"))
	{
		TreeNode* pNode = (TreeNode*)_Param;
		CGameObject* pObject = (CGameObject*)pNode->GetData();

		//CEditorMgr::GetInst()->EditorDeleteObject(pObject);
		pNode->Deselect();
		DeleteObject(pObject);
		RenewLevel();

		ImGui::CloseCurrentPopup();
	}

	if (ImGui::Button("Close"))
	{
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();
}
