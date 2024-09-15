#pragma once
#include <Engine/CScript.h>

class CJellyBombScript :
    public CScript
{
private:
    bool m_IsBomb;
    float m_SaveTime;

    CGameObject* m_RadiusLight;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void BombStart();
    void Bomb();
    
    void CreateLight(float _Radius, Vec3 _Color);
    void DetectDestroy();
    void CreateExplosion();

public:
    CLONE(CJellyBombScript);
    CJellyBombScript();
    CJellyBombScript(const CJellyBombScript& _Origin);
    ~CJellyBombScript();
};

