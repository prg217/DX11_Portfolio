#pragma once
#include <Engine/CScript.h>

class CFlowerLightScript :
    public CScript
{
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    // ���� ��
    void Bloom();
    // ���� ��
    void Fall();

public:
    CLONE(CFlowerLightScript);
    CFlowerLightScript();
    CFlowerLightScript(const CFlowerLightScript& _Origin);
    ~CFlowerLightScript();
};

