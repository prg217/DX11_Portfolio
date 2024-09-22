#pragma once
#include <Engine/CScript.h>

enum class SpitCactusType
{
    Nomal,
    Poison,
};

class CSpitCactusScript :
    public CScript
{
private:
    SpitCactusType m_Type;

    bool m_Attack;
    float m_SaveAttackTime;
    float m_AttackTime;
    bool m_Dead;

    Ptr<CPrefab> m_NeedleObj0;
    Ptr<CPrefab> m_NeedleObj1;
    Ptr<CPrefab> m_NeedleObj2;
    Ptr<CPrefab> m_NeedleObj3;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void SpawnNeedle();

public:
    void Attack();
    void Stop();

    void Dead();

public:
    CLONE(CSpitCactusScript);
    CSpitCactusScript();
    CSpitCactusScript(const CSpitCactusScript& _Origin);
    ~CSpitCactusScript();
};

