#pragma once
#include "EditorUI.h"

class AE_Detail;
class AE_SpriteView;

class AnimationEditor :
    public EditorUI
{
    // 필요한 것
    // 1. 하나의 플립북에 담을 여러 개의 스프라이트
    //  1-1. 스프라이트 하나를 추가하면 다음 스프라이트를 추가할 수 있게 해야 함
    //  1-2. 스프라이트 벡터 맨 마지막을 none으로 바꾼다면 pop... 
    // 2. 추가한 스프라이트들의 순서를 조정
    // 3. 추가한 스프라이트... 마다 구조체? 그거... 몇 초 동안 보여지는지, 위치는 어디인지 
    //  3-1. 스프라이트를 선택하면 그 스프라이트에 맞는 디테일(구조체)이 보여야 함
    // 4. 세이브, 로드 기능

    // 일단 여기에 스프라이트 추가하는거 넣어두고, 스프라이트 뷰에서 이미지들 순서, 플립북...재생 보여주기
private:
    vector<Ptr<CSprite>>    m_vecSprite;

private:
    AE_SpriteView* m_SpriteView;
    AE_Detail* m_Detail;

public:
    AE_SpriteView* GetSpriteView() { return m_SpriteView; }
    AE_Detail* GetDetail() { return m_Detail; }

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

private:
    void Sprite(int _Count);
public:
    void SetSprite(Ptr<CSprite> _Sprite, int _Count);
    void SetSprites(const vector<Ptr<CSprite>>& _Sprites);

private:
    void SelectSprite(DWORD_PTR _ListUI, DWORD_PTR _Count);

public:
    AnimationEditor();
    ~AnimationEditor();
};

