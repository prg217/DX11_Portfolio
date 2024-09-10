#pragma once
#include "ComponentUI.h"
class SpriteComUI :
    public ComponentUI
{
private:
    virtual void Update() override;

public:
    SpriteComUI();
    ~SpriteComUI();
};

