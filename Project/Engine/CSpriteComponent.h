#pragma once
#include "CComponent.h"

#include "CSprite.h"

class CSpriteComponent :
    public CComponent
{
private:
    Ptr<CSprite>            m_Sprite;

    bool                    m_Outline;      // 외곽선 유무
    bool                    m_AddColor;     // 색 추가 유무
    Vec3                    m_Color;        // 색 추가

    bool                    m_UseLight;     // 빛 적용 유무

public:
    virtual void FinalTick() override;

public:
    void AddSprite(Ptr<CSprite> _Sprtie);
    Ptr<CSprite> GetSprite() { return m_Sprite; }
    void Binding();
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void SetOutline(bool _Outline) { m_Outline = _Outline; }
    void AddColor(bool _AddColor, Vec3 Color);
    void SetUseLight(bool _UseLight) { m_UseLight = _UseLight; }

public:
    CLONE(CSpriteComponent);
    CSpriteComponent();
    CSpriteComponent(CSpriteComponent& _Origin);
    ~CSpriteComponent();

};

