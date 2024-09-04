#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

enum class GrassType
{
    Glowing_Purple,
};

class CGrassScript :
    public CScript
{
private:
    GrassType m_GrassType;
    CGameObject* m_GlowingLight;

    float m_GlowingTime;
    float m_SaveTime;
    bool m_GlowingActive;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
private:
    void GlowingPlants();

public:
    void SetGrassType(GrassType _Type) { m_GrassType = _Type; }

public:
    CLONE(CGrassScript);
    CGrassScript();
    CGrassScript(const CGrassScript& _Origin);
    ~CGrassScript();
};

