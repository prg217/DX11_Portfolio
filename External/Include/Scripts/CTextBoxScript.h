#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CTextBoxScript :
    public CScript
{
private:
    bool m_IsName;
    vector<wstring> m_vText;
    vector<int> m_TextIdx;
    vector<int> m_TextLine;

    float m_SaveTime;
    float m_NextTime;

    int m_TextCount;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void IsName() { m_IsName = true; }
    void SetText(int _Idx, int _Line, wstring _Text); // 몇 번째 대화인지, 몇 번째 줄인지, 텍스트

public:
    CLONE(CTextBoxScript);
    CTextBoxScript();
    CTextBoxScript(const CTextBoxScript& _Origin);
    ~CTextBoxScript();
};

