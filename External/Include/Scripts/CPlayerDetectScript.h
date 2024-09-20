#pragma once
#include <Engine/CScript.h>

class CMonsterScript;

class CPlayerDetectScript :
    public CScript
{
private:
    CMonsterScript* m_MonsterScript;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetMonsterScript(CMonsterScript* _Script) { m_MonsterScript = _Script; }

public:
    CLONE(CPlayerDetectScript);
    CPlayerDetectScript();
    CPlayerDetectScript(const CPlayerDetectScript& _Origin);
    ~CPlayerDetectScript();
};

