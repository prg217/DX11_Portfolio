#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CPlayerScript;
enum class OguAniState;
class CLiftScript;

enum class InteractionState
{
    NONE,

    LIFT_START,
};

class CPlayerInteractionScript :
    public CScript
{
private:
    CGameObject*        m_pPlayer;
    CPlayerScript*      m_pPlayerScript;

    OguAniState         m_PlayerCurAS;

    CGameObject*        m_pInteractionObj;
    InteractionState    m_InteractionState;

    CLiftScript*        m_LiftScript;
    bool                m_LiftOK;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void Move();

public:
    CLONE(CPlayerInteractionScript);
    CPlayerInteractionScript();
    //CPlayerInteractionScript(const CPlayerInteractionScript& _Origin);
    ~CPlayerInteractionScript();
};

