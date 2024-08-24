#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

struct tTileInfo
{
    int ImgIdx;
    Ptr<CTexture> tex;
    //int padding[3];
};

class CTileMap :
    public CRenderComponent
{
private:
    bool                    m_SeveralAtlas;     // 여러 개의 Atlas 텍스쳐를 쓰는지 유무

    int                     m_Row;              // TileMap 의 행 숫자
    int                     m_Col;              // TileMap 의 열 숫자
    Vec2                    m_TileSize;         // Tile 1개의 크기

    vector<Ptr<CTexture>>   m_TileAtlas;        // Tile 개별 이미지들을 보유하고 있는 아틀라스 텍스쳐
    int                     m_TileAtlasCount;   
    Vec2                    m_AtlasResolution;  // Atlas 텍스쳐 해상도
    Vec2                    m_AtlasTileSize;    // Atlas 텍스쳐 내에서 타일 1개의 크기
    Vec2                    m_AtlasTileSliceUV; // Atlas 텍스쳐 내에서 타일 1개의 크기를 UV 로 변환한 크기(SliceUV)

    int                     m_AtlasMaxRow;      // Atlas 텍스쳐가 보유하고 있는 타일의 최대 행 숫자
    int                     m_AtlasMaxCol;      // Atlas 텍스쳐가 보유하고 있는 타일의 최대 열 숫자

  
    vector<tTileInfo>       m_vecTileInfo;      // 각각의 타일의 정보
    CStructuredBuffer*      m_Buffer;           // 각각의 타일정보를 t 레지스터로 전달하기 위한 구조화버퍼


public:
    void SetRowCol(UINT _Row, UINT _Col);
    void SetTileSize(Vec2 _Size);
    void SetAtlasTexture(Ptr<CTexture> _Atlas);
    void SetAtlasTileSize(Vec2 _TileSize);
    void SetSeveralAtlas(bool _SeveralAtlas);

    int GetRow() { return m_Row; }
    int GetCol() { return m_Col; }
    Vec2 GetTileSize() { return m_TileSize; }
    bool GetSeveralAtlas() { return m_SeveralAtlas; }
    Vec2 GetAtlasTileSize() { return m_AtlasTileSize; }

private:
    void ChangeTileMapSize();


public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(CTileMap)
    CTileMap();
    CTileMap(const CTileMap& _Origin);
    ~CTileMap();
};

