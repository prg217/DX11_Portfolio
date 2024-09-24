#pragma once
#include <Engine/CScript.h>

enum class BugBossAni
{
    Appeared, // µÓ¿Â
    Idle,
    StandAttack,
};

class CBugBossScript :
    public CScript
{
private:
    CGameObject* m_LightObj;
    CGameObject* m_WhiteObj;
    CGameObject* m_WingObj;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void FlipPlay(int _FliBookIdx, int _FPS, bool _Repeat);

public:
    CLONE(CBugBossScript);
    CBugBossScript();
    CBugBossScript(const CBugBossScript& _Origin);
    ~CBugBossScript();
};

