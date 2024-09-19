#pragma once
#include "ComponentUI.h"
class FlipBookComUI :
    public ComponentUI
{
private:
    vector<Ptr<CFlipBook>> m_vFlipBook;

private:
    virtual void Update() override;

    void FlipBook(int _Count);
    void SelectFlipBook(DWORD_PTR _ListUI, DWORD_PTR _Count);

public:
    FlipBookComUI();
    ~FlipBookComUI();

};

