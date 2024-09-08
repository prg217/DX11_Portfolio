#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

class CTextBoxScript :
    public CScript
{
private:
    bool m_IsName;
    vector<wstring> m_vText;

    float m_SaveTime;
    float m_NextTime;

    int m_TextCount;
    float m_TextPosY;
    int m_TextIdx;

    CGameObject* m_NextObj;

    bool m_IsNext; // 다음으로 넘어갈 때 누르는 결정키라고 표시
    bool m_IsSkip; // 스킵할 때 누르는 결정키라고 표시

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void IsName() { m_IsName = true; }
    void SetText(wstring _Text); 

public:
    CLONE(CTextBoxScript);
    CTextBoxScript();
    CTextBoxScript(const CTextBoxScript& _Origin);
    ~CTextBoxScript();
};

