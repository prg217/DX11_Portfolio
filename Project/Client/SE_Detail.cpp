#include "pch.h"
#include "SE_Detail.h"

#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

#include "SE_AtlasView.h"

#include <Engine/CAssetMgr.h>


SE_Detail::SE_Detail()
	: m_BackGround(Vec2(0, 0))
	, m_Offset(Vec2(0, 0))
{
}

SE_Detail::~SE_Detail()
{
}


void SE_Detail::Init()
{
	
}

void SE_Detail::Update()
{
	Atlas();

	AtlasInfo();

	Save();
}


void SE_Detail::Atlas()
{	
	string TexName;		

	if (nullptr != m_AtlasTex)
		TexName = string(m_AtlasTex->GetKey().begin(), m_AtlasTex->GetKey().end());

	ImGui::Text("Atlas Texture");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AtlasTex", (char*)TexName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				SetAtlasTex((CTexture*)pAsset.Get());				
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##AtlasTexBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Texture");
		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&SE_Detail::SelectTexture);
		pListUI->SetActive(true);
	}
}

void SE_Detail::AtlasInfo()
{
	// 해상도
	UINT width = 0;
	UINT height = 0;

	if (nullptr != m_AtlasTex)
	{
		width = m_AtlasTex->Width();
		height = m_AtlasTex->Height();
	}

	char buff[50] = {};
	sprintf_s(buff, "%d", width);

	ImGui::Text("Width");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureWidth", buff, 50, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(buff, "%d", height);
	ImGui::Text("Height");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureHeight", buff, 50, ImGuiInputTextFlags_ReadOnly);

	// Background 설정
	ImGui::Text("Background");
	ImGui::SameLine(100);
	ImGui::DragFloat2("##Background", m_BackGround);

	// Offset 설정
	ImGui::Text("Offset");
	ImGui::SameLine(100);
	ImGui::DragFloat2("##Offset", m_Offset);
}

void SE_Detail::Save()
{
	if (ImGui::Button("Save##TileMap", ImVec2(50.f, 18.f)))
	{
		CSprite* pSprite = new CSprite;

		pSprite->SetAtlas(m_AtlasTex);

		ImVec2 leftTop = GetAtlasView()->GetMouseLT();
		ImVec2 rightBotton = GetAtlasView()->GetMouseRB();
		Vec2 LT = Vec2(leftTop.x, leftTop.y);
		Vec2 RB = Vec2(rightBotton.x, rightBotton.y);

		Vec2 Slice = RB - LT;

		pSprite->SetLeftTop(LT);
		pSprite->SetSlice(Slice);

		pSprite->SetBackground(m_BackGround);
		pSprite->SetOffset(m_Offset);

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"sprite\0*.sprite";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"sprite\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			pSprite->Save(szSelect);
		}
	}
}

void SE_Detail::SetAtlasTex(Ptr<CTexture> _Tex)
{
	m_AtlasTex = _Tex;
	GetAtlasView()->SetAtlasTex(m_AtlasTex);
}



void SE_Detail::SelectTexture(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_AtlasTex = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	SetAtlasTex(pTex);	
}