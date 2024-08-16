#pragma once
#include "EditorUI.h"

class ListUI :
    public EditorUI
{
private:
    vector<string>  m_vecList;
    int             m_SelectedIdx;
    string          m_SelectedName;


    EditorUI*       m_UI;
    DELEGATE_1      m_MemFunc;
    DELEGATE_2      m_MemFunc2;
    int             m_IntT;

public:
    void AddItem(const string& _item);
    void AddList(const vector<string>& _list);
    void AddDelegate(EditorUI* _UI, DELEGATE_1 _Func)
    {
        m_UI = _UI; 
        m_MemFunc = _Func;
    }

    template <typename T>
    void AddDelegate(EditorUI* _UI, DELEGATE_2 _Func, T _T)
    {
        m_UI = _UI;
        m_MemFunc2 = _Func;

        if (std::is_same<T, int>::value)
        {
            m_IntT = _T;
        }
    }

    string GetSelectName() { return m_SelectedName ; }
    int GetSelectIdx() { return m_SelectedIdx; }

    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    virtual void Update() override;

public:
    ListUI();
    ~ListUI();
};

