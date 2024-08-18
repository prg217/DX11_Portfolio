#pragma once
#include "EditorUI.h"
#include "AnimationEditor.h"

#include <Engine/CAssetMgr.h>

class AE_Sub :
    public EditorUI
{
private:
    AnimationEditor* m_Owner;

public:
    AnimationEditor* GetOwner() { return m_Owner; }
    class AE_Detail* GetDetail() { return m_Owner->GetDetail(); }
    class AE_SpriteView* GetSpriteView() { return m_Owner->GetSpriteView(); }
    void OwnerSetSprites(const vector<Ptr<CSprite>>& _Sprites) { m_Owner->SetSprites(_Sprites); }

public:
    AE_Sub();
    ~AE_Sub();

    friend class AnimationEditor;
};

