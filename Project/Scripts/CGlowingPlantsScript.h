#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CGlowingPlantsScript :
    public CScript
{
private:
    int m_Number; // 넘버에 따라 움직이는게 다르다.
    bool m_MoveStart;
    float m_ZRotation;
    float m_SaveTime;
    float m_ShakingTime;

    CGameObject* m_GlowingObj;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetNumber(int _Num);
    void MoveStart();
    void GlowingEnd();

public:
    CLONE(CGlowingPlantsScript);
    CGlowingPlantsScript();
    CGlowingPlantsScript(const CGlowingPlantsScript& _Origin);
    ~CGlowingPlantsScript();
};

