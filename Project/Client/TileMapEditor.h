#pragma once
#include "EditorUI.h"

class TME_TileMapView;
class TME_SelectTex;
class TME_Detail;

class TileMapEditor :
    public EditorUI
{
    // 1. Ÿ�ϸ� ������ �� �ְ� �ϱ�(�⺻ ��� 10x10, ũ�� 2048 ����)
    //  1-1. Ÿ�ϸ� ��� ����, ũ�� ����
    //  1-2. ó���� �� �ؽ��� ����
    //  1-3. ������ �ȿ��� �����ϱ�
    // 2. Ư�� ������ �ִ� �̹�����(����)�� �̹����� �� �ҷ���
    //  2-1. �̹����� Ŭ���ؼ� �����ϰ� ���� Ŭ���ϸ� �� ���� �� �̹����� ä���
    // 3. ������ Ŭ���ϸ� ����
    // 4. ����, �ҷ����� ���

    // offset���� �̵���ų �� �ְ� �ϱ�

    // ��Ʋ�� �ؽ��� �� �� : ����UI���� �� ���� �̹������� ���� ��, �׸��� Ŭ���� ������ �̹��� �ε��� �ٲ��
    // ��Ʋ�� �ؽ��� ���� �� : ���� �̹����� ���� ��

private:
    TME_TileMapView* m_TileMapView;     // Ÿ�ϸ� ���������� ���� �� ����
    TME_SelectTex* m_SelectTex;         // �ؽ��� ����
    TME_Detail* m_Detail;               // ���, ũ�� ����, �׸��� ���̱�, ����, �ҷ�����

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

