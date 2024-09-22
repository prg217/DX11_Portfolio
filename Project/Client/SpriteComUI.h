#pragma once
#include "ComponentUI.h"
class SpriteComUI :
    public ComponentUI
{
private:
    virtual void Update() override;

    void SelectSprite(DWORD_PTR _ListUI);

public:
    SpriteComUI();
    ~SpriteComUI();
};

