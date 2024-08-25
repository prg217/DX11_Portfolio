#include "pch.h"
#include "TME_Detail.h"

#include "TME_SelectTex.h"
#include "TME_TileMapView.h"

#include <Engine/CPathMgr.h>
#include <Engine/CTileMap.h>

TME_Detail::TME_Detail()
	: m_SeveralAtlas(nullptr)
	, m_Row(10)
	, m_Col(10)
	, m_TileSize(Vec2(2048, 2048))
	, m_AtlasTileSize(Vec2(1, 1))
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
		if (m_Row <= 0)
		{
			m_Row = 1;
		}
		GetTileMapView()->SetRow(m_Row);
	}

	ImGui::Text("Col");
	ImGui::SameLine(150);
	if (ImGui::DragInt("##Col", &m_Col))
	{
		if (m_Col <= 0)
		{
			m_Col = 1;
		}
		GetTileMapView()->SetCol(m_Col);
	}

	// TileSize
	ImGui::Text("TileSize");
	ImGui::SameLine(150);
	if (ImGui::DragFloat2("##TileSize", m_TileSize, 1))
	{
		if (m_TileSize.x <= 0)
		{
			m_TileSize.x = 1;
		}
		if (m_TileSize.y <= 0)
		{
			m_TileSize.y = 1;
		}
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

	// 한 개의 아틀라스 텍스쳐를 쓴다면 나타나는 메뉴
	if (!m_SeveralAtlas)
	{
		ImGui::Text("AtlasTileSize");
		ImGui::SameLine(150);
		ImGui::DragFloat2("##AtlasTileSize", m_AtlasTileSize, 1);
		if (m_AtlasTileSize.x <= 0)
		{
			m_AtlasTileSize.x = 1;
		}
		if (m_AtlasTileSize.y <= 0)
		{
			m_AtlasTileSize.y = 1;
		}
		GetTileMapView()->SetAtlasTileSize(m_AtlasTileSize);
	}

	// 타일맵 저장, 불러오기
	SaveLoad();
}

void TME_Detail::SaveLoad()
{
	if (ImGui::Button("Save##TileMap", ImVec2(50.f, 18.f)))
	{
		CTileMap* pTileMap = new CTileMap;
		pTileMap->SetRowCol(m_Row, m_Col);
		pTileMap->SetTileSize(m_TileSize);
		pTileMap->SetAtlasTileSize(m_AtlasTileSize);

		m_vecTileInfo = GetTileMapView()->GetTileEditInfo();
		for (int i = 0; i < m_Col; i++)
		{
			for (int j = 0; j < m_Row; j++)
			{
				int tileMapIdx = m_Col * j + i;

				pTileMap->SetTileInfo(tileMapIdx, m_vecTileInfo[tileMapIdx].ImgIdx, m_vecTileInfo[tileMapIdx].tex);
			}
		}
		pTileMap->SetSeveralAtlas(m_SeveralAtlas);

		// 한 개의 이미지만 사용할 경우 0번 자리에 사용할 이미지를 넣음
		if (!m_SeveralAtlas)
		{
			pTileMap->SetTileInfo(0, m_vecTileInfo[0].ImgIdx, GetTileMapView()->GetOneTex());
		}

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"tile\0*.tile";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"tile\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			FILE* File = nullptr;
			_wfopen_s(&File, szSelect, L"wb");

			if (nullptr == File)
				return;

			pTileMap->SaveToFile(File);

			fclose(File);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load##TileMap", ImVec2(50.f, 18.f)))
	{
		CTileMap* pTileMap = new CTileMap;

		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"tile\0*.tile";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"tile\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			FILE* File = nullptr;
			_wfopen_s(&File, szSelect, L"rb");

			if (nullptr == File)
				return;

			pTileMap->LoadFromFile(File);

			fclose(File);
		}
		else
		{
			return;
		}

		m_SeveralAtlas = pTileMap->GetSeveralAtlas();
		m_Row = pTileMap->GetRow();
		m_Col = pTileMap->GetCol();
		m_TileSize = pTileMap->GetTileSize();
		m_AtlasTileSize = pTileMap->GetAtlasTileSize();

		for (int i = 0; i < m_Col; i++)
		{
			for (int j = 0; j < m_Row; j++)
			{
				int tileMapIdx = m_Col * j + i;

				m_vecTileInfo.resize(m_Col * m_Row);
				m_vecTileInfo[tileMapIdx].ImgIdx = pTileMap->GetImageIdx(tileMapIdx);
				m_vecTileInfo[tileMapIdx].tex = pTileMap->GetTileInfoTex(tileMapIdx);
			}
		}

		GetTileMapView()->SetTileEditInfo(m_vecTileInfo);
		GetTileMapView()->SetRow(m_Row);
		GetTileMapView()->SetCol(m_Col);
		GetTileMapView()->SetTileSize(m_TileSize);
		GetSelectTex()->SetSeveralAtlas(m_SeveralAtlas);
		GetTileMapView()->SetSeveralAtlas(m_SeveralAtlas);

		if (!m_SeveralAtlas)
		{
			GetTileMapView()->SetAtlasTileSize(m_AtlasTileSize);
			GetSelectTex()->SetSelectTexture(m_vecTileInfo[0].tex);
			GetTileMapView()->SetOneTex(m_vecTileInfo[0].tex);
		}
	}
}
