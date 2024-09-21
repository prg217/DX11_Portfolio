#pragma once
#include <Engine/CScript.h>

enum class NeedleDir
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum class SpitCactusType;

class CCactusNeedleScript :
    public CScript
{
private:
    SpitCactusType m_Type;
    NeedleDir m_Dir;

    float m_Speed;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CCactusNeedleScript);
    CCactusNeedleScript();
    CCactusNeedleScript(const CCactusNeedleScript& _Origin);
    ~CCactusNeedleScript();
};

