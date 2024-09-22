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
    bool                    m_Reverse;      // ����� ����

    bool                    m_Stop;

    bool                    m_Outline;      // �ܰ��� ����
    bool                    m_AddColor;     // �� �߰� ����
    Vec3                    m_Color;        // �� �߰�
    float                   m_Alpha;        // ���İ�

    bool                    m_UseLight;     // �� ���� ����
    
public:
    virtual void FinalTick() override;

public:
    void AddFlipBook(int _Idx, Ptr<CFlipBook> _Flipbook);
    Ptr<CFlipBook> FindFlipBook(const wstring& _Key);
    void Play(int _FliBookIdx, float _FPS, bool _Repeat, int _StartFrmIdx = 0);
    void ReversePlay(int _FliBookIdx, float _FPS, bool _Repeat);
    bool GetIsFinish() { return m_Finish; }
    Ptr<CSprite> GetCurSprite() { return m_CurFrmSprite; }
    int GetCurIdx() { return m_CurFrmIdx; }
    vector<Ptr<CFlipBook>> GetFlipBooks() { return m_vecFlipBook; }
    void SetFlipBook(vector<Ptr<CFlipBook>> _vFlipBook) { m_vecFlipBook = _vFlipBook; }

    void Reset();
    void Binding();
    void Stop();
    static void Clear();
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void SetOutline(bool _Outline) { m_Outline = _Outline; }
    void AddColor(bool _AddColor, Vec3 Color);
    void AddAlpha(float _Alpha) { m_Alpha = _Alpha; }
    void SetUseLight(bool _UseLight) { m_UseLight = _UseLight; }

public:
    CLONE(CFlipBookComponent);
    CFlipBookComponent();
    CFlipBookComponent(CFlipBookComponent& _Origin);
    ~CFlipBookComponent();
};

