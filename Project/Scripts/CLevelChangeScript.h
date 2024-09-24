#pragma once
#include <Engine/CScript.h>

#include <Engine/CLevelMgr.h>

class CLevelChangeScript :
    public CScript
{
private:
    wstring m_LeveName;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void NextLevel();

public:
    CLONE(CLevelChangeScript);
    CLevelChangeScript();
    CLevelChangeScript(const CLevelChangeScript& _Origin);
    ~CLevelChangeScript();
};

