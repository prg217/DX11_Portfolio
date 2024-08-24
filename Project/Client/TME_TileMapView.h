#pragma once
#include "TME_Sub.h"
class TME_TileMapView :
    public TME_Sub
{
private:
    vector<tTileEditInfo>       m_vecTileInfo;      // 각각의 타일의 정보

    bool                        m_SeveralAtlas;

    int                         m_Row;
    int                         m_Col;
    Vec2                        m_TileSize;

    ImVec2          ImageRectMin; // Image 위젯 좌상단 실제 위치

    ImVec2          m_MousePos; // 실제 마우스 위치

    ImVec2          m_MouseLT;  // 마우스 위치에 해당하는 Texture 의 픽셀 좌표
    ImVec2          m_MouseRB; // 마우스 위치에 해당하는 Texture 의 픽셀 좌표

    set<Vec2>       m_PixelID;  // 등록된적이 있는 픽셀인지 확인하는 용도

    float           m_WheelScale;

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void WheelCheck();

public:
    void SetSeveralAtlas(bool _SeveralAtlas);
    void SetRow(int _Row);
    void SetCol(int _Col);
    void SetTileSize(Vec2 _TileSize);

public:
    TME_TileMapView();
    ~TME_TileMapView();
};

