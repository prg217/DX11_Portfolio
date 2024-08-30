#pragma once
#include "SE_Sub.h"


class SE_Detail :
    public SE_Sub
{
private:
    Ptr<CTexture>   m_AtlasTex;

    Vec2            m_BackGround;
    Vec2            m_Offset;

    bool            m_Passivity;

    Vec2            m_LT;
    Vec2            m_RB;

private:
    void Atlas();
    void AtlasInfo();

    void Save();

public:
    void SetAtlasTex(Ptr<CTexture> _Tex);

    Vec2 GetBackGround() { return m_BackGround; }
    Vec2 GetOffset() { return m_Offset; }

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void SelectTexture(DWORD_PTR _ListUI);

public:
    SE_Detail();
    ~SE_Detail();
};

