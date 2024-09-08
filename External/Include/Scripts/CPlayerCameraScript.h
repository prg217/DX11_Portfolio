#pragma once
#include <Engine/CScript.h>

class CCameraPlayerTrackingScript;

class CPlayerCameraScript :
    public CScript
{
private:
    CGameObject* m_pCamera;
    CCameraPlayerTrackingScript* m_pCameraScript;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CPlayerCameraScript);
    CPlayerCameraScript();
    ~CPlayerCameraScript();
};

