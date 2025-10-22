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
	// 선택된 텍스쳐
	if (m_SelectTex != nullptr)
	{
		ImGui::Text("Select");

		// 실제 해상도 대비 출력 Image 의 비율
		float selectRatio = m_WidthSize / m_SelectTex->Width();
		ImGui::Image(m_SelectTex->GetSRV().Get(), ImVec2((m_WidthSize), m_SelectTex->Height() * selectRatio), ImVec2(0, 0), ImVec2(1, 1), tint_col, border_col);

		ImGui::Separator();
	}

	// 텍스쳐들을 불러옴
	vector<string> vecTexNames;
	CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);

	for (int i = 0; i < vecTexNames.size(); i++)
	{
		wstring strAssetName = wstring(vecTexNames[i].begin(), vecTexNames[i].end());
		Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

		assert(pTex.Get());

		// 실제 해상도 대비 출력 Image 의 비율
		float ratio = m_WidthSize / pTex->Width();

		if (ImGui::ImageButton(pTex->GetSRV().Get(), ImVec2((m_WidthSize), pTex->Height() * ratio),
			ImVec2(0, 0), ImVec2(1, 1), -1, tint_col, border_col))
		{
			m_SelectTex = pTex;

			// 한 개의 텍스쳐만 쓴다면 바로 텍스쳐 적용
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
