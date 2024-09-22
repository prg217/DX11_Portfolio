#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CPlayerScript;
enum class OguAniState;

class CPlayerInteractionScript :
    public CScript
{
private:
    CGameObject*        m_pPlayer;
    CPlayerScript*      m_pPlayerScript;

    OguAniState         m_PlayerCurAS;

    CGameObject*        m_pInteractionObj;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void Move();

    void Lift(CGameObject* _Other);
    void LiftEnd(CGameObject* _pInteractionObj);

public:
    CLONE(CPlayerInteractionScript);
    CPlayerInteractionScript();
    //CPlayerInteractionScript(const CPlayerInteractionScript& _Origin);
    ~CPlayerInteractionScript();
};

