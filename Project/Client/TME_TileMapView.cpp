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
	// Ÿ�� ����
	UINT TileCount = m_Col * m_Row;

	// Ÿ�� ������ �����ϴ� ������ ������ ������ Ÿ�ϰ����� �ٸ��� ��������
	if (m_vecTileInfo.size() != TileCount)
	{
		//m_vecTileInfo.clear();
		m_vecTileInfo.resize(TileCount);
	}

	// Ÿ�ϸ� �����ֱ�
	for (int i = 0; i < m_Row; i++)
	{
		for (int j = 0; j < m_Col; j++)
		{
			WheelCheck();

			// ���� ���� ��Ʋ�� �ؽ��ĸ� ���ٸ� ��Ÿ���� �޴�
			if (m_SeveralAtlas)
			{
				SeveralTexView(j, i);
			}
			// �� ���� ��Ʋ�� �ؽ��ĸ� ���ٸ� ��Ÿ���� �޴�
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
	int tileMapIdx = m_Row * _Row + _Col; // ���� Ÿ�ϸ��� �ε���
	int idx = m_vecTileInfo[tileMapIdx].ImgIdx; // ���� Ÿ�ϸ��� �̹��� �ε���

	// �̹��� �ε����� �̹����� ����ϱ� ���� �ʿ��� �̹��� ��� ��ġ
	int imageRow = idx / m_Row;
	int imageCol = idx % m_Row;

	// ���� �ػ� ��� ��� Image �� ����
	float ratio = (m_TileSize.x * m_WheelScale) / m_TileSize.y;

	if (m_OneTex != nullptr)
	{
		ImVec2 leftTopUV = ImVec2(m_AtlasTileSliceUV.x * imageRow, m_AtlasTileSliceUV.y * imageCol);
		ImVec2 rightBottomUV = ImVec2(m_AtlasTileSliceUV.x * (imageRow + 1), m_AtlasTileSliceUV.y * (imageCol + 1));

		// ���� ���̵� �ο�
		ImGui::PushID(tileMapIdx);

		if (ImGui::ImageButton(m_OneTex->GetSRV().Get(), ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio),
			leftTopUV, rightBottomUV, -1, tint_col, border_col))
		{
			// ��ư�� ������ ��� ���� Ÿ�ϸ��� �ε��� ����ü�� ImgIdx�� +1�Ѵ�.
			// ���� m_ImgIdxMax�� �Ѵ´ٸ� 0���� ȸ��
			if (idx > (m_ImgIdxMax - 1))
			{
				m_vecTileInfo[tileMapIdx].ImgIdx = 0;
			}
			else
			{
				m_vecTileInfo[tileMapIdx].ImgIdx++;
			}
		}

		// ���̵� ����
		ImGui::PopID();
	}
	else
	{
		// �ؽ��İ� ������ �� �̹����� �����ش�.
		ImGui::ImageButton(nullptr, ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col);
	}

}

void TME_TileMapView::SeveralTexView(int _Row, int _Col)
{
	int tileMapIdx = m_Row * _Row + _Col; // ���� Ÿ�ϸ��� �ε���
	Ptr<CTexture> tex = m_vecTileInfo[tileMapIdx].tex;

	// ���� �ػ� ��� ��� Image �� ����
	float ratio = (m_TileSize.x * m_WheelScale) / m_TileSize.y;

	// ���� ���̵� �ο�
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
		// �ؽ��İ� ������ �� �̹����� �����ش�.
		if (ImGui::ImageButton(nullptr, ImVec2((m_TileSize.x * m_WheelScale), m_TileSize.y * ratio), uv_min, uv_max, -1, tint_col, border_col))
		{
			m_vecTileInfo[tileMapIdx].tex = GetSelectTex()->GetSelectTexture();
		}
	}

	// ���̵� ����
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
	// �̹��� �ε��� �ִ�ġ
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

