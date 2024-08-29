#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class COguRunParticleScript :
    public CScript
{
private:
    float m_SaveTime;
    float m_DeadTime;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(COguRunParticleScript);
    COguRunParticleScript();
    ~COguRunParticleScript();
};

