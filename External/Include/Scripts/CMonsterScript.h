#pragma once
#include <Engine/CScript.h>

enum class MonsterType
{
    SpitCactus,
};

class CMonsterScript :
    public CScript
{
private:
    CGameObject* m_HPBar;
    CGameObject* m_HPFrame;

    bool m_Hit;
    float m_SaveHitTime;

    MonsterType m_MonsterType;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void PlayerDetect(bool _In);

    void Hit();

public:
    CLONE(CMonsterScript);
    CMonsterScript();
    CMonsterScript(const CMonsterScript& _Origin);
    ~CMonsterScript();
};

