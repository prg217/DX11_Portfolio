#pragma once
#include "CComponent.h"

#include "CSprite.h"

class CSpriteComponent :
    public CComponent
{
private:
    Ptr<CSprite>            m_Sprite;

    bool                    m_Outline;      // �ܰ��� ����
    bool                    m_AddColor;     // �� �߰� ����
    Vec3                    m_Color;        // �� �߰�

    bool                    m_UseLight;     // �� ���� ����
    
    bool                    m_UseSliceAmend;   // �����̽� ���� ����
    Vec2                    m_SliceAmend;

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

    void SliceAmend(bool _Use, Vec2 _Slice = Vec2(0, 0));
    void SliceAmendUV(bool _Use, Vec2 _SliceUV);

    bool GetAddColor() { return m_AddColor; }
    Vec3 GetColor() { return m_Color; }
    bool GetUseLight() { return m_UseLight; }

public:
    CLONE(CSpriteComponent);
    CSpriteComponent();
    CSpriteComponent(CSpriteComponent& _Origin);
    ~CSpriteComponent();

};

