#pragma once
#include "TME_Sub.h"
class TME_Detail :
    public TME_Sub
{
private:
    bool m_SeveralAtlas;
    int m_Row;
    int m_Col;
    Vec2 m_TileSize;
    Vec2 m_AtlasTileSize;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    TME_Detail();
    ~TME_Detail();
};

