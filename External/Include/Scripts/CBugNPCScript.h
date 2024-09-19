#pragma once
#include <Engine/CScript.h>

enum class JellyPushType;

class CBugNPCScript :
    public CScript
{
private:
    JellyPushType m_JellyPushType;

    bool m_SameType;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CBugNPCScript);
    CBugNPCScript();
    CBugNPCScript(const CBugNPCScript& _Origin);
    ~CBugNPCScript();

};

