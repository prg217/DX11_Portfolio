#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class COguDancePointLightScript :
    public CScript
{
private:
    float m_MaxRadius;
    float m_MinRadius;
    float m_Duration; // 최대 범위에서 최소 범위로 될 때까지 걸리는 시간

    float m_Time;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetMaxRadius(float _Max) { m_MaxRadius = _Max; }
    void SetMinRadius(float _Min) { m_MinRadius = _Min; }
    void SetDuration(float _Duration) { m_Duration = _Duration; }

public:
    CLONE(COguDancePointLightScript);
    COguDancePointLightScript();
    COguDancePointLightScript(const COguDancePointLightScript& _Origin);
    ~COguDancePointLightScript();
};

