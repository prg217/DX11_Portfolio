#pragma once
#include "EditorUI.h"

class AE_Detail;
class AE_SpriteView;

class AnimationEditor :
    public EditorUI
{
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

