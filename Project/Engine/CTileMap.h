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
    bool                    m_SeveralAtlas;     // ���� ���� Atlas �ؽ��ĸ� ������ ����

    int                     m_Row;              // TileMap �� �� ����
    int                     m_Col;              // TileMap �� �� ����
    Vec2                    m_TileSize;         // Tile 1���� ũ��

    vector<Ptr<CTexture>>   m_TileAtlas;        // Tile ���� �̹������� �����ϰ� �ִ� ��Ʋ�� �ؽ���
    int                     m_TileAtlasCount;   
    Vec2                    m_AtlasResolution;  // Atlas �ؽ��� �ػ�
    Vec2                    m_AtlasTileSize;    // Atlas �ؽ��� ������ Ÿ�� 1���� ũ��
    Vec2                    m_AtlasTileSliceUV; // Atlas �ؽ��� ������ Ÿ�� 1���� ũ�⸦ UV �� ��ȯ�� ũ��(SliceUV)

    int                     m_AtlasMaxRow;      // Atlas �ؽ��İ� �����ϰ� �ִ� Ÿ���� �ִ� �� ����
    int                     m_AtlasMaxCol;      // Atlas �ؽ��İ� �����ϰ� �ִ� Ÿ���� �ִ� �� ����

  
    vector<tTileInfo>       m_vecTileInfo;      // ������ Ÿ���� ����
    CStructuredBuffer*      m_Buffer;           // ������ Ÿ�������� t �������ͷ� �����ϱ� ���� ����ȭ����


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

