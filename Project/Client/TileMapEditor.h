#pragma once
#include "EditorUI.h"

class TME_TileMapView;
class TME_SelectTex;
class TME_Detail;

class TileMapEditor :
    public EditorUI
{
    // 1. 타일맵 생성할 수 있게 하기(기본 행렬 10x10, 크기 2048 세팅)
    //  1-1. 타일맵 행렬 개수, 크기 지정
    //  1-2. 처음엔 다 텍스쳐 없음
    //  1-3. 에디터 안에서 제작하기
    // 2. 특정 폴더에 있는 이미지들(배경들)을 이미지로 다 불러옴
    //  2-1. 이미지를 클릭해서 선택하고 왼쪽 클릭하면 그 안을 그 이미지로 채우기
    // 3. 오른쪽 클릭하면 삭제
    // 4. 저장, 불러오기 기능

    // offset만들어서 이동시킬 수 있게 하기

    // 아틀라스 텍스쳐 한 개 : 선택UI에서 한 개의 이미지만을 고르게 함, 그리고 클릭할 때마다 이미지 인덱스 바뀌기
    // 아틀라스 텍스쳐 여러 개 : 여러 이미지를 고르게 함

private:
    TME_TileMapView* m_TileMapView;     // 타일맵 직접적으로 보기 및 수정
    TME_SelectTex* m_SelectTex;         // 텍스쳐 선택
    TME_Detail* m_Detail;               // 행렬, 크기 지정, 그리드 보이기, 저장, 불러오기

public:
    TME_TileMapView* GetTileMapView() { return m_TileMapView; }
    TME_SelectTex* GetSelectTex() { return m_SelectTex; }
    TME_Detail* GetDetail() { return m_Detail; }

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    TileMapEditor();
    ~TileMapEditor();
};

