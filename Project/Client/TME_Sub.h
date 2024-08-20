#pragma once
#include "EditorUI.h"

#include "TileMapEditor.h"

class TME_Sub :
    public EditorUI
{
private:
    TileMapEditor* m_Owner;

public:
    TileMapEditor* GetOwner() { return m_Owner; }
    //class AE_Detail* GetDetail() { return m_Owner->GetDetail(); }
    //class AE_SpriteView* GetSpriteView() { return m_Owner->GetSpriteView(); }

public:
    TME_Sub();
    ~TME_Sub();

    friend class TileMapEditor;
};

