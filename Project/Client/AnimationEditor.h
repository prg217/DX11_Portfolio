#pragma once
#include "EditorUI.h"

class AE_Detail;
class AE_SpriteView;

class AnimationEditor :
    public EditorUI
{
    // �ʿ��� ��
    // 1. �ϳ��� �ø��Ͽ� ���� ���� ���� ��������Ʈ
    //  1-1. ��������Ʈ �ϳ��� �߰��ϸ� ���� ��������Ʈ�� �߰��� �� �ְ� �ؾ� ��
    //  1-2. ��������Ʈ ���� �� �������� none���� �ٲ۴ٸ� pop... 
    // 2. �߰��� ��������Ʈ���� ������ ����
    // 3. �߰��� ��������Ʈ... ���� ����ü? �װ�... �� �� ���� ����������, ��ġ�� ������� 
    //  3-1. ��������Ʈ�� �����ϸ� �� ��������Ʈ�� �´� ������(����ü)�� ������ ��
    // 4. ���̺�, �ε� ���

    // �ϴ� ���⿡ ��������Ʈ �߰��ϴ°� �־�ΰ�, ��������Ʈ �信�� �̹����� ����, �ø���...��� �����ֱ�
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

