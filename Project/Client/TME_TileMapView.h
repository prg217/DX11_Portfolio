#pragma once
#include "TME_Sub.h"
class TME_TileMapView :
    public TME_Sub
{
private:
    vector<tTileEditInfo>       m_vecTileInfo;      // ������ Ÿ���� ����

    bool                        m_SeveralAtlas;

    int                         m_Row;
    int                         m_Col;
    Vec2                        m_TileSize;

    ImVec2          ImageRectMin; // Image ���� �»�� ���� ��ġ

    ImVec2          m_MousePos; // ���� ���콺 ��ġ

    ImVec2          m_MouseLT;  // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ
    ImVec2          m_MouseRB; // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ

    set<Vec2>       m_PixelID;  // ��ϵ����� �ִ� �ȼ����� Ȯ���ϴ� �뵵

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

