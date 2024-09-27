#pragma once
#include <Engine/CScript.h>

class CLightBallScript :
    public CScript
{
private:
    float m_Time;
    float m_Speed;

    bool m_Destroy;

    CGameObject* m_Particle;

    bool m_DeadOK;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void Destroy();
    void SetDeadOK(bool _OK) { m_DeadOK = _OK; }

public:
    CLONE(CLightBallScript);
    CLightBallScript();
    CLightBallScript(const CLightBallScript& _Origin);
    ~CLightBallScript();
};

