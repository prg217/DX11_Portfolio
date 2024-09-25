#pragma once
#include <Engine/CScript.h>

enum class BossType
{
    BugBoss,
};

class CBossInScript :
    public CScript
{
private:
    BossType m_BossType;

    CGameObject* m_Door1;
    CGameObject* m_Door2;
    CGameObject* m_Door3;
    float m_SaveDoorPosY;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CBossInScript);
    CBossInScript();
    CBossInScript(const CBossInScript& _Origin);
    ~CBossInScript();
};

