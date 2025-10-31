#pragma once
#include "TME_Sub.h"
class TME_Detail :
    public TME_Sub
{
private:
    vector<tTileEditInfo>   m_vecTileInfo;

    bool                    m_MultipleImg; // 여러 개의 이미지를 쓰는지 체크
    int                     m_Col;
    int                     m_Row;
    Vec2                    m_TileSize;
    Vec2                    m_AtlasTileSize;

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void SaveLoad();

public:
    TME_Detail();
    ~TME_Detail();
};

