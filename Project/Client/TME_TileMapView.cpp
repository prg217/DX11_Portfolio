#include "pch.h"
#include "TME_TileMapView.h"

#include "TME_SelectTex.h"

TME_TileMapView::TME_TileMapView()
	: m_OneTex(nullptr)
	, m_SeveralAtlas(nullptr)
	, m_Col(10)
	, m_Row(10)
	, m_TileSize(Vec2(2048, 2048))
	, m_AtlasTileSize(Vec2(1, 1))
	, m_AtlasTileSliceUV(Vec2(1, 1))
	, m_ImgIdxMax(0)
	, m_WheelScale(1.f)
	, uv_min(ImVec2(0.0f, 0.0f))
	, uv_max(ImVec2(1.0f, 1.0f))
	, tint_col(ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
	, border_col(ImVec4(0.7f, 0.7f, 0.7f, 1.0f))
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
	UINT TileCount = m_Col * m_Row;

	// 타일 정보를 저장하는 벡터의 데이터 개수가 타일개수랑 다르면 리사이즈
	if (m_vecTileInfo.size() != TileCount)
	{
		//m_vecTileInfo.clear();
		m_vecTileInfo.resize(TileCount);
	}

	// 타일맵 보여주기
	for (int i = 0; i < m_Row; i++)
	{
		for (int j = 0; j < m_Col; j++)
		{
			WheelCheck();

			// 여러 개의 아틀라스 텍스쳐를 쓴다면 나타나는 메뉴
			if (m_SeveralAtlas)
			{
				SeveralTexView(j, i);
			}
			// 한 개의 아틀라스 텍스쳐를 쓴다면 나타나는 메뉴
			else
			{
				OneTexView(j, i);
			}

			if (j != m_Col - 1)
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
		m_WheelScale += 0.005f;
	}

	if (0 > ImGui::GetIO().MouseWheel)
	{
		m_WheelScale -= 0.005f;
	}

	if (3.f < m_WheelScale)
		m_WheelScale = 3.f;
	if (m_WheelScale < 0.01f)
		m_WheelScale = 0.01f;
}

void TME_TileMapView::OneTexView(int _Row, int _Col)
{
	int tileMapIdx = m_Row * _Row + _Col; // 현재 타일맵의 인덱스
	int idx = m_vecTileInfo[tileMapIdx].ImgIdx; // 현재 타일맵의 이미지 인덱스

	// 이미지 인덱스의 이미지를 출력하기 위해 필요한 이미지 행렬 위치
	int imageRow = idx / m_Row;
	int imageCol = idx % m_Row;

	// 실제 해상도 대비 출력 Image 의 비율
	float ratio = (m_TileSize.x * m_WheelScale) / m_TileSize.y;

	if (m_OneTex != nullptr)
	{
		ImVec2 leftTopUV = ImVec2(m_AtlasTileSliceUV.x * imageRow, m_AtlasTileSliceUV.y * imageCol);
		ImVec2 rightBottomUV = ImVec2(m_AtlasTileSliceUV.x * (imageRow + 1), m_AtlasTileSliceUV.y * (imageCol + 1));

		// 개별 아이디 부여
		ImGui::PushID(tileMapIdx);

		if (ImGui::ImageButton(m_OneTex->GetSRV().Get(), ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio),
			leftTopUV, rightBottomUV, -1, tint_col, border_col))
		{
			// 버튼을 눌렀을 경우 현재 타일맵의 인덱스 구조체의 ImgIdx를 +1한다.
			// 만약 m_ImgIdxMax가 넘는다면 0으로 회귀
			if (idx > (m_ImgIdxMax - 1))
			{
				m_vecTileInfo[tileMapIdx].ImgIdx = 0;
			}
			else
			{
				m_vecTileInfo[tileMapIdx].ImgIdx++;
			}
		}

		// 아이디 삭제
		ImGui::PopID();
	}
	else
	{
		// 텍스쳐가 없으면 빈 이미지를 보여준다.
		ImGui::ImageButton(nullptr, ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col);
	}

}

void TME_TileMapView::SeveralTexView(int _Row, int _Col)
{
	int tileMapIdx = m_Row * _Row + _Col; // 현재 타일맵의 인덱스
	Ptr<CTexture> tex = m_vecTileInfo[tileMapIdx].tex;

	// 실제 해상도 대비 출력 Image 의 비율
	float ratio = (m_TileSize.x * m_WheelScale) / m_TileSize.y;

	// 개별 아이디 부여
	ImGui::PushID(tileMapIdx);
	if (tex != nullptr)
	{
		if (ImGui::ImageButton(tex->GetSRV().Get(), ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col))
		{
			m_vecTileInfo[tileMapIdx].tex = GetSelectTex()->GetSelectTexture();
		}
	}
	else
	{
		// 텍스쳐가 없으면 빈 이미지를 보여준다.
		if (ImGui::ImageButton(nullptr, ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col))
		{
			m_vecTileInfo[tileMapIdx].tex = GetSelectTex()->GetSelectTexture();
		}
	}

	// 아이디 삭제
	ImGui::PopID();
}

void TME_TileMapView::SetTileEditInfo(vector<tTileEditInfo> _TileInfo)
{
	m_vecTileInfo = _TileInfo;
}

void TME_TileMapView::SetOneTex(Ptr<CTexture> _Tex)
{
	m_OneTex = _Tex;
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

void TME_TileMapView::SetAtlasTileSize(Vec2 _AtlasTileSize)
{
	m_AtlasTileSize = _AtlasTileSize;
	// 이미지 인덱스 최대치
	if (m_OneTex == nullptr)
	{
		return;
	}
	Vec2 atlasResolution = Vec2((float)m_OneTex->Width(), (float)m_OneTex->Height());
	
	m_AtlasTileSliceUV = m_AtlasTileSize / atlasResolution;

	int atlasMaxCol = int(atlasResolution.x / m_AtlasTileSize.x);
	int atlasMaxRow = int(atlasResolution.y / m_AtlasTileSize.y);
	m_ImgIdxMax = m_Row * atlasMaxRow + atlasMaxCol;
}

