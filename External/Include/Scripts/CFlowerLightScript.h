#pragma once
#include <Engine/CScript.h>

enum class JellyPushType;

class CFlowerLightScript :
    public CScript
{
private:
    JellyPushType m_Type; // ¡©∏Æ ªˆªÛ

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    // ≤…¿Ã «À
    void Bloom(JellyPushType _JellyPushType);
    // ≤…¿Ã ¡¸
    void Fall();

public:
    CLONE(CFlowerLightScript);
    CFlowerLightScript();
    CFlowerLightScript(const CFlowerLightScript& _Origin);
    ~CFlowerLightScript();
};

