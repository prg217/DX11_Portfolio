#pragma once
#include "CComponent.h"

#include "CFlipBook.h"

class CFlipBookComponent :
    public CComponent
{
private:
    vector<Ptr<CFlipBook>>  m_vecFlipBook;  // FliBook ������Ʈ�� ������ ��� FlipBook ���
    Ptr<CFlipBook>          m_CurFlipBook;  // ���� ������� FlipBook
    Ptr<CSprite>            m_CurFrmSprite; // ���� ������� FlipBook ���� ���� ������ �ε����� �ش��ϴ� ��������Ʈ
    int                     m_CurFrmIdx;    // ���� ������� FlipBook ���� ���° Sprite �� ��������� �ε��� ���
    float                   m_FPS;          // ���� ������� FlipBook �� �ʴ� ������ ���� ��
    float                   m_AccTime;      // ���� �ð��� üũ
    bool                    m_Repeat;
    bool                    m_Finish;       // FlipBook ����� ���� �����ߴ��� ����
    
public:
    virtual void FinalTick() override;

public:
    void AddFlipBook(int _Idx, Ptr<CFlipBook> _Flipbook);
    Ptr<CFlipBook> FindFlipBook(const wstring& _Key);
    void Play(int _FliBookIdx, float _FPS, bool _Repeat);
    Ptr<CSprite> GetCurSprite() { return m_CurFrmSprite; }
    void Reset();
    void Binding();
    static void Clear();
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CFlipBookComponent);
    CFlipBookComponent();
    CFlipBookComponent(CFlipBookComponent& _Origin);
    ~CFlipBookComponent();
};

