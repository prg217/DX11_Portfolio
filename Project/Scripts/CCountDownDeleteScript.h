#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CCountDownDeleteScript :
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
    void SetSaveTime(float _SaveTime) { m_SaveTime = _SaveTime; }
    void SetDeadTime(float _DeadTime) { m_DeadTime = _DeadTime; }

public:
    CLONE(CCountDownDeleteScript);
    CCountDownDeleteScript();
    CCountDownDeleteScript(const CCountDownDeleteScript& _Origin);
    ~CCountDownDeleteScript();
};

