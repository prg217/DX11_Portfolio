#pragma once
#include "TME_Sub.h"
class TME_SelectTex :
    public TME_Sub
{
private:
    Ptr<CTexture>   m_SelectTex;

    bool            m_SeveralAtlas;

    float           m_WidthSize;
    ImVec4          tint_col;
    ImVec4          border_col;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    void SetSeveralAtlas(bool _SeveralAtlas);
    void SetSelectTexture(Ptr<CTexture> _Tex);

    Ptr<CTexture> GetSelectTexture() { return m_SelectTex; }

public:
    TME_SelectTex();
    ~TME_SelectTex();
};

