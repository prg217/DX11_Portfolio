#pragma once

#include <Engine/singleton.h>

class CGameObject;
class EditorUI;

class CEditorMgr :
    public CSingleton<CEditorMgr>
{
    SINGLE(CEditorMgr);
private:
    vector<CGameObject*>        m_vecEditorObject;
    map<string, EditorUI*>      m_mapUI;

    HANDLE                      m_hNotifyHandle;
    
    vector<CGameObject*>	    m_GC; // Garbage Collector;

public:
    EditorUI* FindEditorUI(const string& Name);


public:
    void Init();
    void Tick();

public:
    void EditorDeleteObject(CGameObject* _Obj);

private:
    void ClearGC();

private:
    void ShortCut();

    void CreateEditorObject();
    void EditorObjectProgress();

    void InitImGui();
    void ObserveContent();
    void ImGuiProgress();
    void ImGuiTick();
    void CreateEditorUI();
};

