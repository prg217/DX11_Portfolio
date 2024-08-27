#pragma once
#include "TME_Sub.h"
class TME_TileMapView :
    public TME_Sub
{
private:
    // =======타일 정보=======
    vector<tTileEditInfo>       m_vecTileInfo;      // 각각의 타일의 정보

    bool                        m_SeveralAtlas;

    int                         m_Row;
    int                         m_Col;
    Vec2                        m_TileSize;
    Vec2                        m_AtlasTileSize;
    // =======================
    // =======한 개의 텍스쳐를 사용할 때 변수=======
    Ptr<CTexture>   m_OneTex;           // 한 개의 텍스쳐만 쓸 때 텍스쳐
    Vec2            m_AtlasTileSliceUV; // Atlas 텍스쳐 내에서 타일 1개의 크기를 UV 로 변환한 크기(SliceUV)
    int             m_ImgIdxMax; // 최대 이미지 인덱스
    // =============================================
    // =======마우스 휠로 이미지 크기 조정하는 변수=======
    ImVec2      ImageRectMin; // Image 위젯 좌상단 실제 위치

    ImVec2      m_MousePos; // 실제 마우스 위치

    ImVec2      m_LT;  // 마우스 위치에 해당하는 Texture 의 픽셀 좌표
    ImVec2      m_RB; // 마우스 위치에 해당하는 Texture 의 픽셀 좌표

    set<Vec2>   m_PixelID;  // 등록된적이 있는 픽셀인지 확인하는 용도

    float       m_WheelScale;
    // ===================================================
    // =======이미지 함수에 필요한 변수=======
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

