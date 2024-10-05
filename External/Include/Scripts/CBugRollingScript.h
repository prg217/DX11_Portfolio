#pragma once
#include <Engine/CScript.h>

enum class BugRollingAni
{
    IDLE_DOWN,
    IDLE_UP,
    IDLE_LEFT,
    IDLE_RIGHT,

    DOWN,
    UP,
    LEFT,
    RIGHT,

    ROLLING_DOWNUP,
    ROLLING_LEFTRIGHT,

    END_DOWN,
    END_UP,
    END_LEFT,
    END_RIGHT,
};

enum class BugRollingDir
{
    NONE,

    DOWN,
    UP,
    LEFT,
    RIGHT,

    LEFTDOWN,
    LEFTUP,
    RIGHTDOWN,
    RIGHTUP,
};

enum class BugRollingState
{
    IDLE,
    ROLLING,
    STUN,
};

class CMonsterScript;
enum class JellyPushType;

class CBugRollingScript :
    public CScript
{
private:
    CMonsterScript* m_pMonsterScript;
    CGameObject* m_pPlayer;
    CGameObject* m_RollParticle;

    float m_Speed;
    
    bool m_End;
    float m_ChaseTime; // 추격 시간
    BugRollingAni m_CurAni;
    BugRollingDir m_CurDir;
    BugRollingState m_CurState;

    JellyPushType m_JellyType;

    bool m_Stun;
    CGameObject* m_StunObj;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void AniChange();
    void Chase(); // 추격
    void Move(); // 이동
    void MoveEndAni();

    void StunEffect();
    void StunDelete();

public:
    void Attack();
    void Stop();
    void Dead();
    void HitEffect();

public:
    CLONE(CBugRollingScript);
    CBugRollingScript();
    CBugRollingScript(const CBugRollingScript& _Origin);
    ~CBugRollingScript();
};

