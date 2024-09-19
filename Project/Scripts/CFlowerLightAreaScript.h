#pragma once
#include <Engine/CScript.h>

enum class JellyPushType;

class CFlowerLightAreaScript :
    public CScript
{
private:
    JellyPushType m_Type;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetJellyPushType(JellyPushType _Type);

    JellyPushType GetJellyPushType() { return m_Type; }

public:
    CLONE(CFlowerLightAreaScript);
    CFlowerLightAreaScript();
    CFlowerLightAreaScript(const CFlowerLightAreaScript& _Origin);
    ~CFlowerLightAreaScript();
};

