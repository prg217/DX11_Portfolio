#pragma once
#include <Engine/CScript.h>

class CHPScript;

class CPlayerHPScript :
    public CScript
{
private:
    CHPScript* m_PlayerHP;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CPlayerHPScript);
    CPlayerHPScript();
    ~CPlayerHPScript();
};

