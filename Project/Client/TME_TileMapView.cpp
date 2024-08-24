#include "pch.h"
#include "TME_TileMapView.h"

TME_TileMapView::TME_TileMapView()
	: m_SeveralAtlas(nullptr)
	, m_Row(10)
	, m_Col(10)
	, m_TileSize(Vec2(2048, 2048))
	, m_WheelScale(1.f)
{
}

TME_TileMapView::~TME_TileMapView()
{
}

void TME_TileMapView::Init()
{
}

void TME_TileMapView::Update()
{
	// Ÿ�� ����
	UINT TileCount = m_Row * m_Col;

	// Ÿ�� ������ �����ϴ� ������ ������ ������ Ÿ�ϰ����� �ٸ��� ��������
	if (m_vecTileInfo.size() != TileCount)
	{
		//m_vecTileInfo.clear();
		m_vecTileInfo.resize(TileCount);
	}

	// Ÿ�ϸ� �����ֱ�
	for (int i = 0; i < m_Col; i++)
	{
		for (int j = 0; j < m_Row; j++)
		{
			WheelCheck();

			// �̹���	
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);

			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

			Ptr<CTexture> tex = nullptr;

			// ���� �ػ� ��� ��� Image �� ����
			float ratio = (m_TileSize.x * m_WheelScale) / m_TileSize.y;

			for (int i = 0; i < m_vecTileInfo.size(); ++i)
			{
				// ���� ���� ��Ʋ�� �ؽ��ĸ� ���ٸ� ��Ÿ���� �޴�
				if (m_SeveralAtlas)
				{
					// �� �̹����� 0�� 0���� �ִ� �̹��� ����?
					tex = m_vecTileInfo[i].tex;
					//ImVec2((m_WidthSize * m_WheelScale), m_AtlasTex->Height() * m_Ratio)
					if (ImGui::ImageButton(tex->GetSRV().Get(), ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col))
					{

					}
				}
				// �� ���� ��Ʋ�� �ؽ��ĸ� ���ٸ� ��Ÿ���� �޴�
				else
				{
					int idx = m_vecTileInfo[i].ImgIdx;
					//tex = ; 

					if (ImGui::ImageButton(nullptr, ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col))
					{

					}
				}
			}

			// �̹����� ������ �� �常 ����ϴ� ��쿡�� �ε����� ����, ���� ���� ���õ� �̹����� ����
			//ImGui::ImageButton(tex->GetSRV().Get(), ImVec2((m_WidthSize), tex->Height() * ratio), leftTopUV, rightDownUV, tint_col, border_col);

			if (j != m_Row - 1)
			{
				ImGui::SameLine();
			}
		}
	}
}

void TME_TileMapView::WheelCheck()
{
	if (0 < ImGui::GetIO().MouseWheel)
	{
		m_WheelScale += 0.05f;
	}

	if (0 > ImGui::GetIO().MouseWheel)
	{
		m_WheelScale -= 0.05f;
	}

	if (3.f < m_WheelScale)
		m_WheelScale = 3.f;
	if (m_WheelScale < 0.1f)
		m_WheelScale = 0.1f;
}

void TME_TileMapView::SetSeveralAtlas(bool _SeveralAtlas)
{
	m_SeveralAtlas = _SeveralAtlas;
}

void TME_TileMapView::SetRow(int _Row)
{
	m_Row = _Row;
}

void TME_TileMapView::SetCol(int _Col)
{
	m_Col = _Col;
}

void TME_TileMapView::SetTileSize(Vec2 _TileSize)
{
	m_TileSize = _TileSize;
}

