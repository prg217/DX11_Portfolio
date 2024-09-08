#pragma once
#include <Engine/CScript.h>

class CPlayerScript;

class CCameraPlayerTrackingScript :
    public CScript
{
private:
    bool   m_IsMove;

    CGameObject* m_pPlayer;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();

public:
    void Stop() { m_IsMove = false; }
    void Move() { m_IsMove = true; }

public:
    CLONE(CCameraPlayerTrackingScript);
    CCameraPlayerTrackingScript();
    ~CCameraPlayerTrackingScript();
};

