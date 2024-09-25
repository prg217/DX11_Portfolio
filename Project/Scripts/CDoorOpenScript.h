#pragma once
#include <Engine/CScript.h>

class CDoorOpenScript :
    public CScript
{
    // 콜라이더에 진입하면 연결된 문들을 연다.
private:
    CGameObject* m_Door1;
    CGameObject* m_Door2;
    CGameObject* m_Door3;

    bool m_Open;
    Vec3 m_Pos;
    float m_GoalPosY;
    float m_Speed;
    Vec2 m_Slice;

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
    CLONE(CDoorOpenScript);
    CDoorOpenScript();
    CDoorOpenScript(const CDoorOpenScript& _Origin);
    ~CDoorOpenScript();
};

