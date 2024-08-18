#pragma once
#include "AE_Sub.h"
class AE_Detail :
    public AE_Sub
{
private:
    vector<Ptr<CSprite>>    m_vecSprite;

    float                   m_WidthSize;

    int                     m_CurSpriteIdx;
    float                   m_FPS;          // ���� ������� FlipBook �� �ʴ� ������ ���� ��
    float                   m_AccTime;      // ���� �ð��� üũ

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

