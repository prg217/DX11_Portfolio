#pragma once
#include <Engine/CScript.h>

enum class PuzzleType
{
    STONE_BLOCK,
    FLOWER,
};

enum class JellyPushType;

class CJellyPushFrameScript :
    public CScript
{
private:
    // �� �������� Ÿ���� ���ؼ� �� Ÿ�Կ� �´� �͸� ���� ����
    JellyPushType m_JellyType;
    PuzzleType m_PuzzleType;
    int m_PuzzleObjNum;
    CGameObject* m_InteractionObj1;
    CGameObject* m_InteractionObj2;

    CGameObject* m_LinkObj;
    bool m_LinkOK;

    // =====STONE_BLOCK���� �ʿ��� ������=====
    bool m_Open;
    Vec3 m_Pos;
    //Vec3 m_Pos2;
    float m_GoalPosY;
    float m_Speed;
    Vec2 m_Slice;
    // =========================================

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void Open();

    void StoneBlock();
    void FlowerBloom(JellyPushType _JellyPushType);
    void FlowerFall();

public:
    bool GetLinkOK() { return m_LinkOK; }

public:
    CLONE(CJellyPushFrameScript);
    CJellyPushFrameScript();
    CJellyPushFrameScript(const CJellyPushFrameScript& _Origin);
    ~CJellyPushFrameScript();
};

