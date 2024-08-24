#pragma once
#include "EditorUI.h"

#include "TileMapEditor.h"

#include <Engine/CTileMap.h>

struct tTileEditInfo
{
    int ImgIdx;
    Ptr<CTexture> tex;
};

class TME_Sub :
    public EditorUI
{
private:
    TileMapEditor* m_Owner;

public:
    TileMapEditor* GetOwner() { return m_Owner; }
    class TME_TileMapView* GetTileMapView() { return m_Owner->GetTileMapView(); }
    class TME_SelectTex* GetSelectTex() { return m_Owner->GetSelectTex(); }
    class TME_Detail* GetDetail() { return m_Owner->GetDetail(); }

public:
    TME_Sub();
    ~TME_Sub();

    friend class TileMapEditor;
};

