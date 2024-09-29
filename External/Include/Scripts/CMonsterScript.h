#pragma once
#include <Engine/CScript.h>

enum class MonsterType
{
    None,

    SpitCactus,
    BugRolling,
    ColorBug,
};

class CHPScript;

class CMonsterScript :
    public CScript
{
private:
    CGameObject* m_HPBar;
    CGameObject* m_HPFrame;

    CHPScript* m_HpScript;

    bool m_HitOK; // 피격 될 수 있는지
    bool m_Hit;
    float m_SaveHitTime;

    MonsterType m_MonsterType;

    //bool m_KnockBackOK; // 넉백 될 수 있는지

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void HitEffect();

public:
    void PlayerDetect(bool _In);

    void Hit();
    void Dead();

    void SetHitOK(bool _OK) { m_HitOK = _OK; }

public:
    CLONE(CMonsterScript);
    CMonsterScript();
    CMonsterScript(const CMonsterScript& _Origin);
    ~CMonsterScript();
};

