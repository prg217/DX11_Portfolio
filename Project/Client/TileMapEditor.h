#pragma once
#include "EditorUI.h"
class TileMapEditor :
    public EditorUI
{
    // 1. 타일맵 에디터를 키면 현재 레벨에 디버그 선 긋기
    //  1-1. Backgroud 스프라이트?들은 2048x2048 픽셀로 있음, 따라서 디버그 선은 2048픽셀만큼 서로 띄우기
    //  1-2. 에디터를 끄면 디버그 선, 임의로 설치한 타일들 사라지게 하기
    // 2. 특정 폴더에 있는 이미지들(배경들)을 이미지로 다 불러옴
    //  2-1. 이미지를 클릭해서 선택하고 레벨에서 디버그 선 안을 왼쪽 클릭하면 그 안을 그 이미지로 채우기
    // 3. 오른쪽 클릭하면 삭제
    // 4. 타일맵 레이어 추가해서 위에 겹쳐 올릴 수 있게 함
    //  4-1. 레이어마다 저장, 불러오기 따로 할 수 있게 하기
    //  4-2. 레이어 선택하고 현재 레이어를 편집할 수 있게
    // 5. 저장, 불러오기 기능(레이어 마다!)

    // 나중에 못가는 곳 막아두게 콜라이더 에디터도 만들어야 할 듯

private:
    //vector<Ptr<CSprite>>    m_vecSprite;

private:
    //AE_SpriteView* m_SpriteView;
    //AE_Detail* m_Detail;

public:
    //AE_SpriteView* GetSpriteView() { return m_SpriteView; }
    //AE_Detail* GetDetail() { return m_Detail; }

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

private:
    //void Sprite(int _Count);
public:
    //void SetSprite(Ptr<CSprite> _Sprite, int _Count);
    //void SetSprites(const vector<Ptr<CSprite>>& _Sprites);

private:
    //void SelectSprite(DWORD_PTR _ListUI, DWORD_PTR _Count);

public:
    TileMapEditor();
    ~TileMapEditor();
};

