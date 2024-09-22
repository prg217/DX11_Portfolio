#pragma once
#include "CComponent.h"

#include "CFlipBook.h"

class CFlipBookComponent :
    public CComponent
{
private:
    vector<Ptr<CFlipBook>>  m_vecFlipBook;  // FliBook 컴포넌트가 보유한 모든 FlipBook 목록
    Ptr<CFlipBook>          m_CurFlipBook;  // 현재 재생중인 FlipBook
    Ptr<CSprite>            m_CurFrmSprite; // 현재 재생중인 FlipBook 에서 현재 프레임 인덱스에 해당하는 스프라이트
    int                     m_CurFrmIdx;    // 현재 재생중인 FlipBook 에서 몇번째 Sprite 가 재생중인지 인덱스 기록
    float                   m_FPS;          // 현재 재생중인 FlipBook 의 초당 프레임 진행 수
    float                   m_AccTime;      // 누적 시간값 체크
    bool                    m_Repeat;
    bool                    m_Finish;       // FlipBook 재생이 끝에 도달했는지 여부
    bool                    m_Reverse;      // 역재생 여부

    bool                    m_Stop;

    bool                    m_Outline;      // 외곽선 유무
    bool                    m_AddColor;     // 색 추가 유무
    Vec3                    m_Color;        // 색 추가
    float                   m_Alpha;        // 알파값

    bool                    m_UseLight;     // 빛 적용 유무
    
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

