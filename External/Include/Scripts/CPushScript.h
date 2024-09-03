#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CPlayerScript;
enum class OguAniState;

class CPushScript :
    public CScript
{
private:
    CGameObject* m_pPlayer;
    CPlayerScript* m_pPlayerScript;

    float m_PlayerSpeed;
    OguAniState m_PlayerCurAS;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CPushScript);
    CPushScript();
    CPushScript(const CPushScript& _Origin);
    ~CPushScript();
};

