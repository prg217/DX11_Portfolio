#pragma once
#include "SE_Sub.h"

class SE_AtlasView :
    public SE_Sub
{
private:
    Ptr<CTexture>   m_AtlasTex;

    ImVec2          ImageRectMin; // Image 위젯 좌상단 실제 위치

    ImVec2          m_MousePos; // 실제 마우스 위치

    ImVec2          m_LT;  // 마우스 위치에 해당하는 Texture 의 픽셀 좌표
    ImVec2          m_RB; // 마우스 위치에 해당하는 Texture 의 픽셀 좌표

    set<Vec2>       m_PixelID;  // 등록된적이 있는 픽셀인지 확인하는 용도
    
    
    float           m_WidthSize;
    float           m_WheelScale;
    float           m_Ratio;        // 실제 해상도 대비 출력 이미지의 비율

    bool            m_Passivity;


public:
    ImVec2 GetLT() { return m_LT; }
    ImVec2 GetRB() { return m_RB; }
    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    
    void SetAtlasTex(Ptr<CTexture> _Tex);
    void SetPassivity(bool _Passivity);
    void SetLT(ImVec2 _LT);
    void SetRB(ImVec2 _RB);

private:
    void WheelCheck();
    void SelectCheck();
    void DrawSelectRect();
    void CalcSpriteSize(Vec2 _PixelPos);
    bool IsPixelOk(Vec2 _PixelPos);

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    SE_AtlasView();
    ~SE_AtlasView();
};

