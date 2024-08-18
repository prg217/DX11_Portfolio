#pragma once
#include "AE_Sub.h"
class AE_Detail :
    public AE_Sub
{
private:
    vector<Ptr<CSprite>>    m_vecSprite;

    float                   m_WidthSize;

    int                     m_CurSpriteIdx;
    float                   m_FPS;          // 현재 재생중인 FlipBook 의 초당 프레임 진행 수
    float                   m_AccTime;      // 누적 시간값 체크

public:
    void SetSprites(const vector<Ptr<CSprite>>& _Sprites);

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void AniPreview();

public:
    AE_Detail();
    ~AE_Detail();
};

