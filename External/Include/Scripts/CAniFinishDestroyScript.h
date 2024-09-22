#pragma once
#include <Engine/CScript.h>

class CAniFinishDestroyScript :
    public CScript
{
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CAniFinishDestroyScript);
    CAniFinishDestroyScript();
    CAniFinishDestroyScript(const CAniFinishDestroyScript& _Origin);
    ~CAniFinishDestroyScript();
};

