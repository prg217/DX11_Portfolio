#pragma once
#include "EditorUI.h"
class TileMapEditor :
    public EditorUI
{
    // 1. Ÿ�ϸ� �����͸� Ű�� ���� ������ ����� �� �߱�
    //  1-1. Backgroud ��������Ʈ?���� 2048x2048 �ȼ��� ����, ���� ����� ���� 2048�ȼ���ŭ ���� ����
    //  1-2. �����͸� ���� ����� ��, ���Ƿ� ��ġ�� Ÿ�ϵ� ������� �ϱ�
    // 2. Ư�� ������ �ִ� �̹�����(����)�� �̹����� �� �ҷ���
    //  2-1. �̹����� Ŭ���ؼ� �����ϰ� �������� ����� �� ���� ���� Ŭ���ϸ� �� ���� �� �̹����� ä���
    // 3. ������ Ŭ���ϸ� ����
    // 4. Ÿ�ϸ� ���̾� �߰��ؼ� ���� ���� �ø� �� �ְ� ��
    //  4-1. ���̾�� ����, �ҷ����� ���� �� �� �ְ� �ϱ�
    //  4-2. ���̾� �����ϰ� ���� ���̾ ������ �� �ְ�
    // 5. ����, �ҷ����� ���(���̾� ����!)

    // ���߿� ������ �� ���Ƶΰ� �ݶ��̴� �����͵� ������ �� ��

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

