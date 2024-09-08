#pragma once
#include <Engine/CScript.h>

enum class CameraDontMove
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class CPlayerScript;

class CCameraPlayerTrackingScript :
    public CScript
{
private:
    bool   m_IsMove;

    CGameObject* m_pPlayer;
    CameraDontMove m_Dir;

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
    void Stop(CameraDontMove _Dir) { m_IsMove = false; m_Dir = _Dir; }
    void Move() { m_IsMove = true; }

public:
    CLONE(CCameraPlayerTrackingScript);
    CCameraPlayerTrackingScript();
    ~CCameraPlayerTrackingScript();
};

