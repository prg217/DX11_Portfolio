#pragma once
#include <Engine/CScript.h>

class CTextBoxScript;

class CNPCScript :
    public CScript
{
private:
    wstring m_NPCName;
    wstring m_SaveTextFileName;

    CGameObject* m_pTextBox;
    CGameObject* m_pTextBoxName;

    CTextBoxScript* m_pTextBoxScript;
    CTextBoxScript* m_pTextBoxNameScript;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void ActiveTextBox();

public:
    void SetName(wstring _Text);
    void LoadText(const wstring& _FileName);

public:
    CLONE(CNPCScript);
    CNPCScript();
    CNPCScript(const CNPCScript& _Origin);
    ~CNPCScript();
};

