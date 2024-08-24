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
	// 타일 개수
	UINT TileCount = m_Row * m_Col;

	// 타일 정보를 저장하는 벡터의 데이터 개수가 타일개수랑 다르면 리사이즈
	if (m_vecTileInfo.size() != TileCount)
	{
		//m_vecTileInfo.clear();
		m_vecTileInfo.resize(TileCount);
	}

	// 타일맵 보여주기
	for (int i = 0; i < m_Col; i++)
	{
		for (int j = 0; j < m_Row; j++)
		{
			WheelCheck();

			// 이미지	
			ImVec2 uv_min = ImVec2(0.0f, 0.0f);
			ImVec2 uv_max = ImVec2(1.0f, 1.0f);

			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

			Ptr<CTexture> tex = nullptr;

			// 실제 해상도 대비 출력 Image 의 비율
			float ratio = (m_TileSize.x * m_WheelScale) / m_TileSize.y;

			for (int i = 0; i < m_vecTileInfo.size(); ++i)
			{
				// 여러 개의 아틀라스 텍스쳐를 쓴다면 나타나는 메뉴
				if (m_SeveralAtlas)
				{
					// 빈 이미지면 0행 0열에 있는 이미지 쓰기?
					tex = m_vecTileInfo[i].tex;
					//ImVec2((m_WidthSize * m_WheelScale), m_AtlasTex->Height() * m_Ratio)
					if (ImGui::ImageButton(tex->GetSRV().Get(), ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col))
					{

					}
				}
				// 한 개의 아틀라스 텍스쳐를 쓴다면 나타나는 메뉴
				else
				{
					int idx = m_vecTileInfo[i].ImgIdx;
					//tex = ; 

					if (ImGui::ImageButton(nullptr, ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col))
					{

					}
				}
			}

			// 이미지를 누르면 한 장만 사용하는 경우에는 인덱스가 변경, 여러 장은 선택된 이미지로 변경
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

