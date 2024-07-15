#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "ComponentUI.h"
#include "AssetUI.h"
#include "CEditorMgr.h"
#include "ListUI.h"

Inspector::Inspector()
	: m_TargetObject(nullptr)
	, m_arrComUI{}
	, m_arrAssetUI{}
{
	
}

Inspector::~Inspector()
{
}

void Inspector::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;
			
		m_arrComUI[i]->SetTargetObject(_Object);
	}

	m_TargetAsset = nullptr;
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(nullptr);
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	if (nullptr == _Asset)
		return;

	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;
	
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(m_TargetAsset);
	}
}


void Inspector::Update()
{
	if (nullptr == m_TargetObject)
		return;

	// ===========
	// Object Name
	// ===========
	string strObjectName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	string saveObjName = strObjectName;
	ImGui::Text("Object Name");
	ImGui::SameLine(108);		
	//ImGui::InputText("##ObjectName", (char*)strObjectName.c_str(), strObjectName.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::InputText("##ObjectName", (char*)strObjectName.c_str(), strObjectName.length());

	// 오브젝트 이름 바꾸기
	if (saveObjName != strObjectName)
	{
		wstring changeObjName;
		changeObjName.assign(strObjectName.begin(), strObjectName.end());
		m_TargetObject->SetName(changeObjName);

		CLevelMgr::GetInst()->LevelChanged();
	}

	// ======
	// Layer
	// ======
	int LayerIdx = m_TargetObject->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);	
	string LayerName = string(pLayer->GetName().begin(), pLayer->GetName().end());

	char buffer[50] = {};

	if(LayerName.empty())
		sprintf_s(buffer, 50, "%d : %s", LayerIdx, "None");
	else
		sprintf_s(buffer, 50, "%d : %s", LayerIdx, LayerName.c_str());

	ImGui::Text("Layer");
	ImGui::SameLine(108);
	ImGui::InputText("##LayerName", buffer, strlen(buffer), ImGuiInputTextFlags_ReadOnly);

	// 레이어 변경
	ImGui::SameLine();
	if (ImGui::Button("##LayerBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Layer");
		vector<string> vecLayerNames;
		// 레이어 목록
		for (int i = 0; i < MAX_LAYER; i++)
		{
			CLayer* pLayerIdx = pCurLevel->GetLayer(i);
			vecLayerNames.push_back(string(pLayerIdx->GetName().begin(), pLayerIdx->GetName().end()));
		}
		pListUI->AddList(vecLayerNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&Inspector::SelectLayer);
		pListUI->SetActive(true);
	}
}

void Inspector::SelectLayer(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	int selectIdx = pListUI->GetSelectIdx();

	int LayerIdx = m_TargetObject->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);

	pLayer->LayerChange(m_TargetObject, selectIdx);
}