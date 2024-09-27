#pragma once
#include <Engine/CScript.h>

enum class ColorBugType
{
    Blue,
    Green,
    Red,
};

class CColorBugScript :
    public CScript
{
private:
    ColorBugType m_Type;
    
    float m_SaveTime;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void Dead();

public:
    CLONE(CColorBugScript);
    CColorBugScript();
    CColorBugScript(const CColorBugScript& _Origin);
    ~CColorBugScript();
};

