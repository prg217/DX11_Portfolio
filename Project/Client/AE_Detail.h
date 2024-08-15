#pragma once
#include "AE_Sub.h"
class AE_Detail :
    public AE_Sub
{
private:
    Ptr<CTexture>   m_AtlasTex;

private:
    //void Atlas();
    //void AtlasInfo();

public:
    //void SetAtlasTex(Ptr<CTexture> _Tex);

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    //void SelectTexture(DWORD_PTR _ListUI);

public:
    AE_Detail();
    ~AE_Detail();
};

