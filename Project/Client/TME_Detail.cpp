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

	// SeveralAtlas ���� ���� ��Ʋ�� �ؽ��ĸ� ������ üũ
	ImGui::Text("SeveralAtlas");
	ImGui::SameLine(150);
	if (ImGui::Checkbox("##SeveralAtlas", &m_SeveralAtlas))
	{
		// ���� �� ���� ����Ǹ� �ٸ� TME_Sub UI�ʿ��� ����
		GetSelectTex()->SetSeveralAtlas(m_SeveralAtlas);
		GetTileMapView()->SetSeveralAtlas(m_SeveralAtlas);
	}

	ImGui::Text("");
	ImGui::Separator();

	// ���� ���� ��Ʋ�� �ؽ��ĸ� ���ٸ� ��Ÿ���� �޴�
	if (m_SeveralAtlas)
	{
		// tex �������ְ� �ؾ� ��
	}
	// �� ���� ��Ʋ�� �ؽ��ĸ� ���ٸ� ��Ÿ���� �޴�
	else
	{
		ImGui::Text("AtlasTileSize");
		ImGui::SameLine(150);
		ImGui::DragFloat2("##AtlasTileSize", m_AtlasTileSize, 1);

		// �̹��� �ε��� �������ְ� �ؾ� ��
	}

	// Ÿ�ϸ� ����, �ҷ�����
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
		// Ž��â �ʱ� ��ġ ����
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
		// Ž��â �ʱ� ��ġ ����
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
