#pragma once
#include <Engine/CScript.h>

class CBugBallScript :
    public CScript
{
private:
    CGameObject* m_Boss;

    float m_Time;
    float m_Speed;

    bool m_Destroy;

    CGameObject* m_Particle;

    bool m_Hit;
    
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void Destroy();
    void Hit();

public:
    CLONE(CBugBallScript);
    CBugBallScript();
    CBugBallScript(const CBugBallScript& _Origin);
    ~CBugBallScript();
};

