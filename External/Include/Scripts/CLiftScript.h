#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CPlayerScript;
class CInteractionScript;
enum class OguAniState;

class CLiftScript :
    public CScript
{
private:
    CGameObject*    m_pPlayer;
    CPlayerScript*  m_pPlayerScript;

    OguAniState     m_PlayerCurAS;

    bool            m_Start;
    bool            m_End;

    CInteractionScript* m_pInteractionScript;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void Start();
    void End();

    void Starting();
    void Moving();
    void Ending();

public:
    CLONE(CLiftScript);
    CLiftScript();
    CLiftScript(const CLiftScript& _Origin);
    ~CLiftScript();
};

