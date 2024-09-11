#pragma once
#include <Engine/CScript.h>

enum class JellyPushType
{
    CYAN,
    MAGENTA,
    YELLOW,

    BLUE,   // cyan & magenta
    GREEN,  // cyan & yellow
    RED,    // magenta & yellow
};

class CJellyPushScript :
    public CScript
{
private:
    JellyPushType m_Type;

    bool m_IsOverlap;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CreateBlue();

public:
    JellyPushType GetJellyPushType() { return m_Type; }
    bool IsOverlap() { return m_IsOverlap; }

public:
    CLONE(CJellyPushScript);
    CJellyPushScript();
    CJellyPushScript(const CJellyPushScript& _Origin);
    ~CJellyPushScript();
};

