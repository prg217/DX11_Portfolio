#pragma once
#include "EditorUI.h"
class Content :
    public EditorUI
{
private:
    class TreeUI*   m_Tree;
    vector<wstring> m_vecAssetPath; // Content ������ �ִ� ���� ���

private:
    void RenewContent();
    void AssetClicked(DWORD_PTR _Param);
    void PopupMenu(DWORD_PTR _Parma);

    void FindAssetName(const wstring& _FolderPath, const wstring& _Filter);
    void LoadAsset(const path& _Path);    

public:
    // Content ������ �ִ� ���ҽ�(����) ���� Ȯ�� �� �ε�
    void Reload();

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    Content();
    ~Content();
};

