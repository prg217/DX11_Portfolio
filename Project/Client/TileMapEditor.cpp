#include "pch.h"
#include "TileMapEditor.h"

#include "CEditorMgr.h"
#include "TME_Detail.h"
#include "TME_SelectTex.h"
#include "TME_TileMapView.h"

TileMapEditor::TileMapEditor()
	: m_TileMapView(nullptr)
	, m_SelectTex(nullptr)
	, m_Detail(nullptr)
{
	UseMenuBar(true);
}

TileMapEditor::~TileMapEditor()
{
}

void TileMapEditor::Init()
{
	m_TileMapView = (TME_TileMapView*)CEditorMgr::GetInst()->FindEditorUI("TME_TileMapView");
	m_SelectTex = (TME_SelectTex*)CEditorMgr::GetInst()->FindEditorUI("TME_SelectTex");
	m_Detail = (TME_Detail*)CEditorMgr::GetInst()->FindEditorUI("TME_Detail");

	m_TileMapView->SetMove(true);
	m_SelectTex->SetMove(true);
	m_Detail->SetMove(true);

	m_TileMapView->m_Owner = this;
	m_SelectTex->m_Owner = this;
	m_Detail->m_Owner = this;
}

void TileMapEditor::Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool TileMapView = m_TileMapView->IsActive();
			bool SelectTex = m_SelectTex->IsActive();
			bool Detail = m_Detail->IsActive();

			if (ImGui::MenuItem("SpriteView", nullptr, &TileMapView))
			{
				m_TileMapView->SetActive(TileMapView);
			}
			if (ImGui::MenuItem("SpriteView", nullptr, &SelectTex))
			{
				m_SelectTex->SetActive(SelectTex);
			}
			if (ImGui::MenuItem("Detail", nullptr, &Detail))
			{
				m_Detail->SetActive(Detail);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void TileMapEditor::Activate()
{
	m_TileMapView->SetActive(true);
	m_SelectTex->SetActive(true);
	m_Detail->SetActive(true);
}

void TileMapEditor::Deactivate()
{
	m_TileMapView->SetActive(false);
	m_SelectTex->SetActive(false);
	m_Detail->SetActive(false);
}