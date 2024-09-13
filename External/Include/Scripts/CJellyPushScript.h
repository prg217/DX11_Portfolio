#pragma once
#include <Engine/CScript.h>

enum class JellyPushType
{
    NONE,

    CYAN,
    MAGENTA,
    YELLOW,

    BLUE,   // cyan & magenta
    GREEN,  // cyan & yellow
    RED,    // magenta & yellow
};

class CJellyPushScript :
    public CScript
{
private:
    JellyPushType m_Type;
    JellyPushType m_OtherObjType;

    bool m_IsOverlap;
    CGameObject* m_Destination;
    float m_Speed;
    Vec3 m_OtherPos;

    float m_SaveSpawnTime;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void SetDestinationMove(CGameObject* _Destination); // 목적지까지 이동 셋팅
    void DestinationMove();
    void CreateBigJellyPush(JellyPushType _Type);
    CGameObject* CreateMiniJellyPush(JellyPushType _Type);
    void CreateParticle();

public:
    JellyPushType GetJellyPushType() { return m_Type; }
    bool IsOverlap() { return m_IsOverlap; }

    CGameObject* Speparation(); // 큰 색 젤리(JellyPush) 분리

    void SetType(JellyPushType _Type) { m_Type = _Type; }

public:
    CLONE(CJellyPushScript);
    CJellyPushScript();
    CJellyPushScript(const CJellyPushScript& _Origin);
    ~CJellyPushScript();
};

