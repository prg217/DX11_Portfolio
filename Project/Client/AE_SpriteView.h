#pragma once
#include "AE_Sub.h"
class AE_SpriteView :
    public AE_Sub
{
private:
    vector<Ptr<CSprite>>    m_vecSprite;

    float                   m_WidthSize;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    void SetSprites(vector<Ptr<CSprite>>  _Sprites);

public:
    AE_SpriteView();
    ~AE_SpriteView();
};

