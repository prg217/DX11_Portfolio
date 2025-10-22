#include "pch.h"
#include "TME_SelectTex.h"

#include <Engine/CAssetMgr.h>

#include "TME_TileMapView.h"

TME_SelectTex::TME_SelectTex()
	: m_SelectTex(nullptr)
	, m_SeveralAtlas(false)
	, m_WidthSize(80)
{
}

TME_SelectTex::~TME_SelectTex()
{
}

void TME_SelectTex::Init()
{
	tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
}

void TME_SelectTex::Update()
{
	// ���õ� �ؽ���
	if (m_SelectTex != nullptr)
	{
		ImGui::Text("Select");

		// ���� �ػ� ��� ��� Image �� ����
		float selectRatio = m_WidthSize / m_SelectTex->Width();
		ImGui::Image(m_SelectTex->GetSRV().Get(), ImVec2((m_WidthSize), m_SelectTex->Height() * selectRatio), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);

		ImGui::Separator();
	}

	// �ؽ��ĵ��� �ҷ���
	vector<string> vecTexNames;
	CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);

	for (int i = 0; i < vecTexNames.size(); i++)
	{
		wstring strAssetName = wstring(vecTexNames[i].begin(), vecTexNames[i].end());
		Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

		assert(pTex.Get());

		// ���� �ػ� ��� ��� Image �� ����
		float ratio = m_WidthSize / pTex->Width();

		if (ImGui::ImageButton(pTex->GetSRV().Get(), ImVec2((m_WidthSize), pTex->Height() * ratio),
			ImVec2(0, 0), ImVec2(1, 1), -1, tint_col, border_col))
		{
			m_SelectTex = pTex;

			// �� ���� �ؽ��ĸ� ���ٸ� �ٷ� �ؽ��� ����
			if (!m_SeveralAtlas)
			{
				GetTileMapView()->SetOneTex(m_SelectTex);
			}
		}

		if (i % 3 != 0)
		{
			ImGui::SameLine();
		}
	}
}

void TME_SelectTex::SetSeveralAtlas(bool _SeveralAtlas)
{
	m_SeveralAtlas = _SeveralAtlas;
}

void TME_SelectTex::SetSelectTexture(Ptr<CTexture> _Tex)
{
	m_SelectTex = _Tex;
}
