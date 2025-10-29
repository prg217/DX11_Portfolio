#pragma once
#include <Engine/CScript.h>

class CUIScript :
    public CScript
{
public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void UIActive(bool _Active);

public:
    CLONE(CUIScript);
    CUIScript();
    CUIScript(const CUIScript& _Origin);
    ~CUIScript();
};

