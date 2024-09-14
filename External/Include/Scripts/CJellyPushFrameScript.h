#pragma once
#include <Engine/CScript.h>

enum class JellyPushType;

class CJellyPushFrameScript :
    public CScript
{
private:
    // 이 프레임의 타입을 정해서 그 타입에 맞는 것만 정답 판정->길 열기
    JellyPushType m_Type;
    int m_PuzzleNum;
    CGameObject* m_StoneBlock;

    bool m_Open;
    Vec3 m_Pos;
    float m_GoalPosY;
    float m_Speed;

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

public:
    CLONE(CJellyPushFrameScript);
    CJellyPushFrameScript();
    CJellyPushFrameScript(const CJellyPushFrameScript& _Origin);
    ~CJellyPushFrameScript();
};

