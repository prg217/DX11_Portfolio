#pragma once
#include <Engine/CScript.h>

class CHPScript :
    public CScript
{
private:
    int m_HP;
    int m_MaxHP;

    CGameObject* m_HPBar;

    bool m_Player;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void Dead();

public:
    void Hit(int _Damage = 1, CGameObject* _HPBar = nullptr);
    void Heal(int _Heal = 1);

    int GetHP() { return m_HP; }
    int GetMaxHP() { return m_MaxHP; }

    void SetMaxHP(int _maxHP) { m_MaxHP = _maxHP; }
    void SetHP(int _HP) { m_HP = _HP; }

public:
    CLONE(CHPScript);
    CHPScript();
    CHPScript(const CHPScript& _Origin);
    ~CHPScript();
};

