#include "pch.h"
#include "CTileMap.h"

#include "CTransform.h"
#include "CAssetMgr.h"

#include "CStructuredBuffer.h"

CTileMap::CTileMap()
	: CRenderComponent(COMPONENT_TYPE::TILEMAP)
	, m_SeveralAtlas(false)
	, m_Row(1)
	, m_Col(1)
	, m_AtlasMaxRow(0)
	, m_AtlasMaxCol(0)	
	, m_Buffer(nullptr)
	, m_TileAtlasCount(0)
{
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"TileMapMtrl"));	

	m_Buffer = new CStructuredBuffer;
}

CTileMap::CTileMap(const CTileMap& _Origin)
	: CRenderComponent(_Origin)
	, m_SeveralAtlas(_Origin.m_SeveralAtlas)
	, m_Row(_Origin.m_Row)
	, m_Col(_Origin.m_Col)
	, m_TileSize(_Origin.m_TileSize)
	, m_TileAtlas(_Origin.m_TileAtlas)
	, m_AtlasResolution(_Origin.m_AtlasResolution)
	, m_AtlasTileSize(_Origin.m_AtlasTileSize)
	, m_AtlasTileSliceUV(_Origin.m_AtlasTileSliceUV)
	, m_AtlasMaxRow(_Origin.m_AtlasMaxRow)
	, m_AtlasMaxCol(_Origin.m_AtlasMaxCol)
	, m_vecTileInfo(_Origin.m_vecTileInfo)
	, m_Buffer(nullptr)
	, m_TileAtlasCount(_Origin.m_TileAtlasCount)
{
	m_Buffer = new CStructuredBuffer;
}

CTileMap::~CTileMap()
{
	delete m_Buffer;
}

void CTileMap::Init()
{
	// 행, 렬 설정해서 구조화버퍼 크기 조정
	SetRowCol(m_Row, m_Col);
}

void CTileMap::FinalTick()
{
}

void CTileMap::Render()
{
	// 타일의 정보를 구조화버퍼를 통해서 t 레지스터에 바인딩 시킨다.
	m_Buffer->SetData(m_vecTileInfo.data(), sizeof(tTileInfo) * m_Row * m_Col);
	m_Buffer->Binding(15);

	if (m_TileAtlasCount < m_TileAtlas.size())
	{
		GetMaterial()->SetTexParam(TEX_0, m_TileAtlas[m_TileAtlasCount++]);
	}
	else
	{
		m_TileAtlasCount = 0;
		if (m_TileAtlasCount < m_TileAtlas.size())
		{
			GetMaterial()->SetTexParam(TEX_0, m_TileAtlas[m_TileAtlasCount++]);
		}
	}
	GetMaterial()->SetScalarParam(INT_1, m_AtlasMaxRow);
	GetMaterial()->SetScalarParam(INT_2, m_AtlasMaxCol);
	GetMaterial()->SetScalarParam(VEC2_1, Vec2(m_Col, m_Row));
	if (!m_SeveralAtlas)
	{
		GetMaterial()->SetScalarParam(VEC2_0, m_AtlasTileSliceUV);
	}
	GetMaterial()->Binding();
	Transform()->Binding();
	GetMesh()->Render();
}

void CTileMap::SetRowCol(UINT _Row, UINT _Col)
{
	m_Row = _Row;
	m_Col = _Col;

	ChangeTileMapSize();

	// 타일 개수
	UINT TileCount = m_Row * m_Col;

	// 타일 정보를 저장하는 벡터의 데이터 개수가 타일개수랑 다르면 리사이즈
	if (m_vecTileInfo.size() != TileCount)
	{
		m_vecTileInfo.clear();
		m_vecTileInfo.resize(TileCount);
	}

	// 타일정보를 전달받아서 t 레지스터에 전달시킬 구조화버퍼가 타일 전체 데이터 사이즈보다 작으면 리사이즈
	if (m_Buffer->GetElementCount() < TileCount)
	{
		m_Buffer->Create(sizeof(tTileInfo), TileCount, SB_TYPE::SRV_ONLY, false);
	}
}

void CTileMap::SetTileSize(Vec2 _Size)
{
	m_TileSize = _Size;

	ChangeTileMapSize();
}

void CTileMap::ChangeTileMapSize()
{
	Vec2 vSize = m_TileSize * Vec2((float)m_Col, (float)m_Row);

	if (GetOwner() != nullptr)
	{
		Transform()->SetRelativeScale(vSize.x, vSize.y, 1.f);
	}
}

void CTileMap::SetAtlasTexture(Ptr<CTexture> _Atlas)
{
	if (m_TileAtlas.size() > 0 && !m_SeveralAtlas)
	{
		m_TileAtlas[0] = _Atlas;
	}
	else
	{
		m_TileAtlas.push_back(_Atlas);
	}

	if (nullptr == m_TileAtlas[m_TileAtlas.size() - 1])
		m_AtlasResolution = Vec2(0.f, 0.f);
	else
		m_AtlasResolution = Vec2((float)_Atlas->Width(), (float)_Atlas->Height());

	SetAtlasTileSize(m_AtlasTileSize);
}

void CTileMap::SetAtlasTileSize(Vec2 _TileSize)
{
	m_AtlasTileSize = _TileSize;

	if (m_SeveralAtlas || m_TileAtlas.size() <= 0)
	{
		return;
	}

	if (nullptr != m_TileAtlas[0])
	{
		m_AtlasTileSliceUV = m_AtlasTileSize / m_AtlasResolution;

		m_AtlasMaxCol = int(m_AtlasResolution.x / m_AtlasTileSize.x);
		m_AtlasMaxRow = int(m_AtlasResolution.y / m_AtlasTileSize.y);
	}
}

void CTileMap::SetSeveralAtlas(bool _SeveralAtlas)
{
	m_SeveralAtlas = _SeveralAtlas;
}

void CTileMap::SetTileInfo(int _TileMapIdx, int _ImgIdx, Ptr<CTexture> _Tex)
{
	m_vecTileInfo[_TileMapIdx].ImgIdx = _ImgIdx;
	//m_vecTileInfo[tileMapIdx].tex = _Tex;

	// 현재 타일맵에는 구조체 tex가 아닌 m_TileAtlas벡터로...이미지 하고 있음
	// 또, nullptr일 때는 어떻게 해야하는지
	//m_TileAtlas.clear();
	//m_TileAtlas
	if (_TileMapIdx >= m_TileAtlas.size())
	{
		m_TileAtlas.resize(_TileMapIdx + 1);
	}

	m_TileAtlas[_TileMapIdx] = _Tex;
}

void CTileMap::SaveToFile(FILE* _File)
{
	SaveDataToFile(_File);

	fwrite(&m_Col, sizeof(int), 1, _File);
	fwrite(&m_Row, sizeof(int), 1, _File);

	fwrite(&m_TileSize, sizeof(Vec2), 1, _File);
	fwrite(&m_AtlasTileSize, sizeof(Vec2), 1, _File);
	
	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		fwrite(&m_vecTileInfo[i], sizeof(tTileInfo), 1, _File);
	}

	fwrite(&m_SeveralAtlas, sizeof(bool), 1, _File);

	// 아틀라스 텍스쳐
	if (m_SeveralAtlas)
	{
		m_AtlasVecSize = m_TileAtlas.size();
		fwrite(&m_AtlasVecSize, sizeof(int), 1, _File);

		for (auto i : m_TileAtlas)
		{
			SaveAssetRef(i, _File);
		}
	}
	else
	{
		if (m_TileAtlas.size() != 0)
		{
			SaveAssetRef(m_TileAtlas[0], _File);
		}
	}
}

void CTileMap::LoadFromFile(FILE* _File)
{
	LoadDataFromFile(_File);

	fread(&m_Col, sizeof(int), 1, _File);
	fread(&m_Row, sizeof(int), 1, _File);

	SetRowCol(m_Row, m_Col);

	fread(&m_TileSize, sizeof(Vec2), 1, _File);
	fread(&m_AtlasTileSize, sizeof(Vec2), 1, _File);

	SetTileSize(m_TileSize);

	for (size_t i = 0; i < m_vecTileInfo.size(); ++i)
	{
		fread(&m_vecTileInfo[i], sizeof(tTileInfo), 1, _File);
	}

	fread(&m_SeveralAtlas, sizeof(bool), 1, _File);

	// 아틀라스 텍스쳐
	if (m_SeveralAtlas)
	{
		fread(&m_AtlasVecSize, sizeof(int), 1, _File);
		m_TileAtlas.clear();
		m_TileAtlas.resize(m_AtlasVecSize);

		for (int i = 0; i < m_AtlasVecSize; i++)
		{
			LoadAssetRef(m_TileAtlas[i], _File);
			SetAtlasTexture(m_TileAtlas[i]);
		}
	}
	else
	{
		if (m_TileAtlas.size() == 0)
		{
			m_TileAtlas.push_back(nullptr);
		}

		LoadAssetRef(m_TileAtlas[0], _File);
		if (nullptr != m_TileAtlas[0])
		{
			SetAtlasTexture(m_TileAtlas[0]);
		}
	}
}