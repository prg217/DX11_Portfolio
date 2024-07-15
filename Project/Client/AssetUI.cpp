#include "pch.h"
#include "AssetUI.h"

#include <Engine/CLevelMgr.h>

AssetUI::AssetUI(ASSET_TYPE _Type)
	: m_Type(_Type)
{
}

AssetUI::~AssetUI()
{
}

void AssetUI::SetAsset(Ptr<CAsset> _Asset)
{
	m_Asset = nullptr;

	if (nullptr == _Asset || m_Type != _Asset->GetAssetType())
	{
		SetActive(false);
	}
	else
	{
		m_Asset = _Asset;
		SetActive(true);		
	}
}

void AssetUI::Title()
{
	ImGui::PushID((int)m_Type);

	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(38, 98, 241, 255));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(38, 98, 241, 255));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(38, 98, 241, 255));
	
	ImGui::Button(ToString(m_Type));

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void AssetUI::OutputAssetName()
{
	Ptr<CAsset> pAsset = GetAsset();

	string assetKey = string(pAsset->GetKey().begin(), pAsset->GetKey().end());
	string saveKey = assetKey;

	// 에셋 키
	ImGui::Text("Key");
	ImGui::SameLine(100);
	ImGui::InputText("##AssetKey", (char*)assetKey.c_str(), assetKey.length(), ImGuiInputTextFlags_ReadOnly);
	//ImGui::InputText("##AssetKey", (char*)assetKey.c_str(), assetKey.length());

	// 에셋 키 바꾸기(해도 되나 고민 중)
	// 1. 중복되는 키가 있나 확인 후 있으면 어떻게 할 것인가?
	// (에셋 매니저에서 FindAsset 후 중복되는 것이 있으면...)
	//	1-a. 이름을 다시 원상 복귀 후 리턴
	//	1-b. 중복되는 이름이 있다고 경고창 띄우기
	// 2. 키를 바꾸면 Save, Load는?
	//	2-a. 기존에 있던 저장 파일 삭제 후 다시 저장
	/*
	if (saveKey != assetKey)
	{
		wstring changeKey;
		changeKey.assign(assetKey.begin(), assetKey.end());
		pAsset->ChangeKey(changeKey);

		//CLevelMgr::GetInst()->LevelChanged();
		//CAssetMgr::GetInst()->
	}
	*/
}
