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

	// ���� Ű
	ImGui::Text("Key");
	ImGui::SameLine(100);
	ImGui::InputText("##AssetKey", (char*)assetKey.c_str(), assetKey.length(), ImGuiInputTextFlags_ReadOnly);
	//ImGui::InputText("##AssetKey", (char*)assetKey.c_str(), assetKey.length());

	// ���� Ű �ٲٱ�(�ص� �ǳ� ��� ��)
	// 1. �ߺ��Ǵ� Ű�� �ֳ� Ȯ�� �� ������ ��� �� ���ΰ�?
	// (���� �Ŵ������� FindAsset �� �ߺ��Ǵ� ���� ������...)
	//	1-a. �̸��� �ٽ� ���� ���� �� ����
	//	1-b. �ߺ��Ǵ� �̸��� �ִٰ� ���â ����
	// 2. Ű�� �ٲٸ� Save, Load��?
	//	2-a. ������ �ִ� ���� ���� ���� �� �ٽ� ����
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
