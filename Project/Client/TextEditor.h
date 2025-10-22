#pragma once
#include "EditorUI.h"
#include <locale>
#include <codecvt>

class TextEditor :
    public EditorUI
{
private:
    int m_textSize;
    vector<string> m_vText;

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

private:
    void AddText(int _Index);

    void SaveLoad();

public:
    TextEditor();
    ~TextEditor();
};

