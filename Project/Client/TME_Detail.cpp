#include "pch.h"
#include "TME_Detail.h"

#include "TME_SelectTex.h"
#include "TME_TileMapView.h"

TME_Detail::TME_Detail()
	: m_SeveralAtlas(nullptr)
	, m_Row(10)
	, m_Col(10)
	, m_TileSize(Vec2(2048, 2048))
	, m_AtlasTileSize(1, 1)
{
}

TME_Detail::~TME_Detail()
{
}

void TME_Detail::Init()
{
}

void TME_Detail::Update()
{
	// RowCol
	ImGui::Text("Row");
	ImGui::SameLine(150);
	if (ImGui::DragInt("##Row", &m_Row))
	{
		GetTileMapView()->SetRow(m_Row);
	}

	ImGui::Text("Col");
	ImGui::SameLine(150);
	if (ImGui::DragInt("##Col", &m_Col))
	{
		GetTileMapView()->SetCol(m_Col);
	}

	// TileSize
	ImGui::Text("TileSize");
	ImGui::SameLine(150);
	if (ImGui::DragFloat2("##TileSize", m_TileSize, 1))
	{
		GetTileMapView()->SetTileSize(m_TileSize);
	}

	// SeveralAtlas 여러 개의 아틀라스 텍스쳐를 쓰는지 체크
	ImGui::Text("SeveralAtlas");
	ImGui::SameLine(150);
	if (ImGui::Checkbox("##SeveralAtlas", &m_SeveralAtlas))
	{
		// 만약 이 값이 변경되면 다른 TME_Sub UI쪽에도 전달
		GetSelectTex()->SetSeveralAtlas(m_SeveralAtlas);
		GetTileMapView()->SetSeveralAtlas(m_SeveralAtlas);
	}

	ImGui::Text("");
	ImGui::Separator();

	// 여러 개의 아틀라스 텍스쳐를 쓴다면 나타나는 메뉴
	if (m_SeveralAtlas)
	{
		// tex 변경해주게 해야 함
	}
	// 한 개의 아틀라스 텍스쳐를 쓴다면 나타나는 메뉴
	else
	{
		ImGui::Text("AtlasTileSize");
		ImGui::SameLine(150);
		ImGui::DragFloat2("##AtlasTileSize", m_AtlasTileSize, 1);

		// 이미지 인덱스 변경해주게 해야 함
	}

	// 타일맵 저장, 불러오기
	/*
	if (ImGui::Button("Save##FlipBook", ImVec2(50.f, 18.f)))
	{
		CFlipBook* pFlipBook = new CFlipBook;
		pFlipBook->SetSprites(m_vecSprite);

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"Animation\0*.flip";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"Animation\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			pFlipBook->Save(szSelect);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load##FlipBook", ImVec2(50.f, 18.f)))
	{
		CFlipBook* pFlipBook = new CFlipBook;

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"Animation\0*.flip";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"Animation\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			pFlipBook->Load(szSelect);
		}

		m_vecSprite = pFlipBook->GetSprites();
		OwnerSetSprites(m_vecSprite);
		GetSpriteView()->SetSprites(m_vecSprite);
	}
	*/
}
