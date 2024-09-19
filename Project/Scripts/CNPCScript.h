#pragma once
#include <Engine/CScript.h>

class CNPCScript :
    public CScript
{
private:
    vector<wstring> m_Text;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetText(wstring _Text);
    void TextClear() { m_Text.clear(); }

public:
    CLONE(CNPCScript);
    CNPCScript();
    CNPCScript(const CNPCScript& _Origin);
    ~CNPCScript();
};

