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
    float  m_Speed;

    CGameObject* m_pFocusObj;
    CameraDontMove m_Dir;

    bool m_Shaking;
    float m_ShakingTime;

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

    void Focus(CGameObject* _FocusObj) { m_pFocusObj = _FocusObj; }
    
    void Shaking();

    void SetCameraSpeed(float _Speed) { m_Speed = _Speed; }

public:
    CLONE(CCameraPlayerTrackingScript);
    CCameraPlayerTrackingScript();
    ~CCameraPlayerTrackingScript();
};

