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
	, m_Passivity(false)
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

	// 미리보기
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	ImGui::Text("Preview");
	Ptr<CTexture> tex = GetAtlasView()->GetAtlasTex();
	m_LT = Vec2(GetAtlasView()->GetLT().x, GetAtlasView()->GetLT().y);
	m_RB = Vec2(GetAtlasView()->GetRB().x, GetAtlasView()->GetRB().y);
	Vec2 tex_size = Vec2(tex->Width(), tex->Height());
	Vec2 center = Vec2((m_LT.x + m_RB.x - m_Offset.x) / 2, (m_LT.y + m_RB.y - m_Offset.y) / 2);
	ImVec2 LT = ImVec2((center.x - m_BackGround.x) / tex_size.x, (center.y - m_BackGround.y) / tex_size.y);
	ImVec2 RB = ImVec2((center.x + m_BackGround.x) / tex_size.x, (center.y + m_BackGround.y) / tex_size.y);
	ImGui::Image(tex->GetSRV().Get(), ImVec2(m_BackGround.x, m_BackGround.y), LT, RB, tint_col, border_col);
	// 현재 윈도우의 드로우 리스트를 가져옵니다.
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	// 이미지의 화면 위치를 가져옵니다.
	ImVec2 image_pos = ImGui::GetCursorScreenPos();
	// 이미지의 중앙 좌표 계산
	ImVec2 image_center = ImVec2(image_pos.x + m_BackGround.x * 0.5f, image_pos.y - m_BackGround.y * 0.5f); 

	// 십자선의 길이와 색상을 정의합니다.
	float crosshair_length = 20.0f; // 십자선의 길이
	ImU32 line_color = IM_COL32(255, 0, 0, 255); // 선의 색상 (빨간색)

	// 이미지 중앙에 십자선을 그립니다.
	draw_list->AddLine(ImVec2(image_center.x - crosshair_length, image_center.y), ImVec2(image_center.x + crosshair_length, image_center.y), line_color, 2.0f); // 수평선
	draw_list->AddLine(ImVec2(image_center.x, image_center.y - crosshair_length), ImVec2(image_center.x, image_center.y + crosshair_length), line_color, 2.0f); // 수직선

	ImGui::Separator();

	ImGui::Text("Passivity");
	ImGui::SameLine(100);
	if (ImGui::Checkbox("##Passivity", &m_Passivity))
	{
		GetAtlasView()->SetPassivity(m_Passivity);
	}

	if (m_Passivity)
	{
		// LT
		ImGui::Text("Left Top");
		ImGui::SameLine(100);
		if (ImGui::DragFloat2("##LT", m_LT))
		{
			GetAtlasView()->SetLT(ImVec2(m_LT.x, m_LT.y));
		}

		// RB
		ImGui::Text("Right Bottom");
		ImGui::SameLine(100);
		if (ImGui::DragFloat2("##RB", m_RB))
		{
			GetAtlasView()->SetRB(ImVec2(m_RB.x, m_RB.y));
		}
	}
}

void SE_Detail::Save()
{
	if (ImGui::Button("Save##TileMap", ImVec2(50.f, 18.f)))
	{
		CSprite* pSprite = new CSprite;

		pSprite->SetAtlas(m_AtlasTex);

		if (!m_Passivity)
		{
			ImVec2 leftTop = GetAtlasView()->GetLT();
			ImVec2 rightBotton = GetAtlasView()->GetRB();

			m_LT = Vec2(leftTop.x, leftTop.y);
			m_RB = Vec2(rightBotton.x, rightBotton.y);
		}

		Vec2 Slice = m_RB - m_LT;

		pSprite->SetLeftTop(m_LT);
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