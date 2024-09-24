#pragma once
#include <Engine/CScript.h>

class CLevelChangeScript :
    public CScript
{
private:


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void ChangeLevel();

public:
    CLONE(CLevelChangeScript);
    CLevelChangeScript();
    CLevelChangeScript(const CLevelChangeScript& _Origin);
    ~CLevelChangeScript();
};

