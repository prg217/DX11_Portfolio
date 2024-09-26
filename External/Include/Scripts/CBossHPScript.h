#pragma once
#include <Engine/CScript.h>

class CBossHPScript :
    public CScript
{
private:
    int m_X;
    int m_Y;

    bool m_Name;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void Start();

public:
    CLONE(CBossHPScript);
    CBossHPScript();
    CBossHPScript(const CBossHPScript& _Origin);
    ~CBossHPScript();
};

