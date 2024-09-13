#pragma once
#include <Engine/CScript.h>

enum class JellyPushType;

class CJellyPushFrameScript :
    public CScript
{
private:
    // �� �������� Ÿ���� ���ؼ� �� Ÿ�Կ� �´� �͸� ���� ����->�� ����
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
    CLONE(CJellyPushFrameScript);
    CJellyPushFrameScript();
    CJellyPushFrameScript(const CJellyPushFrameScript& _Origin);
    ~CJellyPushFrameScript();
};

