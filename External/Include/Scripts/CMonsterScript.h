#pragma once
#include <Engine/CScript.h>

class CMonsterScript :
    public CScript
{
private:
    CGameObject* m_HPBar;

    bool m_Hit;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void PlayerDetect(bool _In);

    void Hit();

public:
    CLONE(CMonsterScript);
    CMonsterScript();
    CMonsterScript(const CMonsterScript& _Origin);
    ~CMonsterScript();
};

