#pragma once
#include <Engine/CScript.h>

class CNPCScript :
    public CScript
{
private:
    vector<wstring> m_vText;
    wstring m_NPCName;

    int m_textSize;

    CGameObject* m_pTextBox;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void CreateTextBox();

public:
    void SetName(wstring _Text);
    void SetText(wstring _Text);
    void LoadText(const wstring& _FileName);
    void TextClear() { m_vText.clear(); }

public:
    CLONE(CNPCScript);
    CNPCScript();
    CNPCScript(const CNPCScript& _Origin);
    ~CNPCScript();
};

