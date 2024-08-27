#pragma once
#include "TME_Sub.h"
class TME_TileMapView :
    public TME_Sub
{
private:
    // =======Ÿ�� ����=======
    vector<tTileEditInfo>       m_vecTileInfo;      // ������ Ÿ���� ����

    bool                        m_SeveralAtlas;

    int                         m_Row;
    int                         m_Col;
    Vec2                        m_TileSize;
    Vec2                        m_AtlasTileSize;
    // =======================
    // =======�� ���� �ؽ��ĸ� ����� �� ����=======
    Ptr<CTexture>   m_OneTex;           // �� ���� �ؽ��ĸ� �� �� �ؽ���
    Vec2            m_AtlasTileSliceUV; // Atlas �ؽ��� ������ Ÿ�� 1���� ũ�⸦ UV �� ��ȯ�� ũ��(SliceUV)
    int             m_ImgIdxMax; // �ִ� �̹��� �ε���
    // =============================================
    // =======���콺 �ٷ� �̹��� ũ�� �����ϴ� ����=======
    ImVec2      ImageRectMin; // Image ���� �»�� ���� ��ġ

    ImVec2      m_MousePos; // ���� ���콺 ��ġ

    ImVec2      m_LT;  // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ
    ImVec2      m_RB; // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ

    set<Vec2>   m_PixelID;  // ��ϵ����� �ִ� �ȼ����� Ȯ���ϴ� �뵵

    float       m_WheelScale;
    // ===================================================
    // =======�̹��� �Լ��� �ʿ��� ����=======
    ImVec2 uv_min;
    ImVec2 uv_max;

    ImVec4 tint_col;
    ImVec4 border_col;
    // =======================================
public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void WheelCheck();
    void OneTexView(int _Row, int _Col);
    void SeveralTexView(int _Row, int _Col);

public:
    vector<tTileEditInfo> GetTileEditInfo() { return m_vecTileInfo; }
    Ptr<CTexture> GetOneTex() { return m_OneTex; }

    void SetTileEditInfo(vector<tTileEditInfo> _TileInfo);
    void SetOneTex(Ptr<CTexture> _Tex);
    void SetSeveralAtlas(bool _SeveralAtlas);
    void SetRow(int _Row);
    void SetCol(int _Col);
    void SetTileSize(Vec2 _TileSize);
    void SetAtlasTileSize(Vec2 _AtlasTileSize);

public:
    TME_TileMapView();
    ~TME_TileMapView();
};

