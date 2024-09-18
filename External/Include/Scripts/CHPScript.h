#pragma once
#include <Engine/CScript.h>

class CHPScript :
    public CScript
{
private:
    int m_HP;
    int m_MaxHP;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void Dead();

public:
    void Hit(int _Damage = 1);
    void Heal(int _Heal = 1);

public:
    CLONE(CHPScript);
    CHPScript();
    CHPScript(const CHPScript& _Origin);
    ~CHPScript();
};

