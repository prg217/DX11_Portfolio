#pragma once
#include "ComponentUI.h"
class RigidBodyUI :
    public ComponentUI
{
private:
    virtual void Update() override;

public:
    RigidBodyUI();
    ~RigidBodyUI();
};

