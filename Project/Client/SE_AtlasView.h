#pragma once
#include "SE_Sub.h"

class SE_AtlasView :
    public SE_Sub
{
private:
    Ptr<CTexture>   m_AtlasTex;

    ImVec2          ImageRectMin; // Image ���� �»�� ���� ��ġ

    ImVec2          m_MousePos; // ���� ���콺 ��ġ

    ImVec2          m_LT;  // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ
    ImVec2          m_RB; // ���콺 ��ġ�� �ش��ϴ� Texture �� �ȼ� ��ǥ

    set<Vec2>       m_PixelID;  // ��ϵ����� �ִ� �ȼ����� Ȯ���ϴ� �뵵
    
    
    float           m_WidthSize;
    float           m_WheelScale;
    float           m_Ratio;        // ���� �ػ� ��� ��� �̹����� ����

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

