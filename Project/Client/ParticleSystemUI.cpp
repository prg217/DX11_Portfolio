#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>

#include "TreeUI.h"
#include "ListUI.h"
#include "CEditorMgr.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_Particle(nullptr)
	, m_WidthSize(50)
	, m_Ratio(0)
{
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::Update()
{
	Title();

	m_Particle = GetTargetObject()->ParticleSystem();
	m_ParticleModule = m_Particle->GetParticleModule();

	// 파티클 저장, 불러오기
	SaveLoad();

	// 파티클 이미지
	m_ParticleTex = m_Particle->GetParticleTexture();

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	if (m_ParticleTex == nullptr)
	{
		ImGui::Image(nullptr, ImVec2(m_WidthSize, m_WidthSize), uv_min, uv_max, tint_col, border_col);
	}
	else
	{
		m_Ratio = m_WidthSize / m_ParticleTex->Width();
		ImGui::Image(m_ParticleTex->GetSRV().Get(), ImVec2(m_WidthSize, m_ParticleTex->Height() * m_Ratio), uv_min, uv_max, tint_col, border_col);
	}

	string TexName;

	if (nullptr != m_ParticleTex)
		TexName = string(m_ParticleTex->GetKey().begin(), m_ParticleTex->GetKey().end());

	ImGui::Text("Particle Texture");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##ParticleTex", (char*)TexName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				SetParticleTex((CTexture*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##ParticleTexBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Texture");
		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&ParticleSystemUI::SelectTexture);
		pListUI->SetActive(true);
	}

	//m_Particle->SetParticleTexture(m_ParticleTex);

	// Spawn Module
	ImGui::Text("Spawn Module");

	ImGui::Separator();

	ImGui::Text("Active");
	ImGui::SameLine(100);
	bool SpawnModuleActive = m_ParticleModule.Module[(UINT)PARTICLE_MODULE::SPAWN];
	if (ImGui::Checkbox("##SpawnModuleActive", &SpawnModuleActive))
	{
		m_ParticleModule.Module[(UINT)PARTICLE_MODULE::SPAWN] = SpawnModuleActive;
	}

	ImGui::Text("SpawnRate");
	ImGui::SameLine(100);
	int SpawnRate = m_ParticleModule.SpawnRate;
	if (ImGui::DragInt("##SpawnRate", &SpawnRate))
	{
		m_ParticleModule.SpawnRate = SpawnRate;
	}

	ImGui::Text("SpawnColor");
	ImGui::SameLine(100);
	Vec4 vSpawnColor = m_ParticleModule.vSpawnColor;
	if (ImGui::ColorEdit4("##SpawnColor", vSpawnColor))
	{
		m_ParticleModule.vSpawnColor = vSpawnColor;
	}

	ImGui::Text("MinLife");
	ImGui::SameLine(100);
	ImGui::DragFloat("##MinLife", &m_ParticleModule.MinLife);

	ImGui::Text("MaxLife");
	ImGui::SameLine(100);
	ImGui::DragFloat("##MaxLife", &m_ParticleModule.MaxLife);

	ImGui::Text("SpawnMinScale");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##SpawnMinScale", m_ParticleModule.vSpawnMinScale);

	ImGui::Text("SpawnMaxScale");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##SpawnMaxScale", m_ParticleModule.vSpawnMaxScale);

	ImGui::Text("SpawnShape");
	int SpawnShape = m_ParticleModule.SpawnShape;
	if (ImGui::RadioButton("Box##SpawnShape", SpawnShape == 0))
	{
		SpawnShape = 0;
	}
	if (ImGui::RadioButton("Sphere##SpawnShape", SpawnShape == 1))
	{
		SpawnShape = 1;
	}
	m_ParticleModule.SpawnShape = SpawnShape;

	ImGui::Text("SpawnShapeScale X");
	ImGui::SameLine(100);
	ImGui::DragFloat("##SpawnShapeScaleX", &m_ParticleModule.SpawnShapeScale.x);

	ImGui::Text("BlockSpawnShape");
	int BlockSpawnShape = m_ParticleModule.BlockSpawnShape;
	if (ImGui::RadioButton("Box##BlockSpawnShape", BlockSpawnShape == 0))
	{
		BlockSpawnShape = 0;
	}
	if (ImGui::RadioButton("Sphere##BlockSpawnShape", BlockSpawnShape == 1))
	{
		BlockSpawnShape = 1;
	}
	m_ParticleModule.BlockSpawnShape = BlockSpawnShape;

	ImGui::Text("BlockSpawnShapeScale X");
	ImGui::SameLine(100);
	ImGui::DragFloat("##BlockSpawnShapeScaleX", &m_ParticleModule.BlockSpawnShapeScale.x);

	ImGui::Text("SpaceType");
	int SpaceType = m_ParticleModule.SpaceType;
	if (ImGui::RadioButton("LocalSpace", SpaceType == 0))
	{
		SpaceType = 0;
	}
	if (ImGui::RadioButton("WorldSpace", SpaceType == 1))
	{
		SpaceType = 1;
	}
	m_ParticleModule.SpaceType = SpaceType;

	ImGui::Text("");
	ImGui::Separator();

	// Spawn Burst Module
	ImGui::Text("Spawn Burst Module");

	ImGui::Separator();

	ImGui::Text("Active");
	ImGui::SameLine(100);
	bool SpawnBurstModuleActive = m_ParticleModule.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST];
	if (ImGui::Checkbox("##SpawnBurstModuleActive", &SpawnBurstModuleActive))
	{
		m_ParticleModule.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = SpawnBurstModuleActive;
	}

	ImGui::Text("SpawnBurstRepeat");
	ImGui::SameLine(100);
	bool SpawnBurstRepeat = m_ParticleModule.SpawnBurstRepeat;
	if (ImGui::Checkbox("##SpawnBurstRepeat", &SpawnBurstRepeat))
	{
		m_ParticleModule.SpawnBurstRepeat = SpawnBurstRepeat;
	}

	ImGui::Text("SpawnBurstCount");
	ImGui::SameLine(100);
	int SpawnBurstCount = m_ParticleModule.SpawnBurstCount;
	if (ImGui::DragInt("##SpawnBurstCount", &SpawnBurstCount))
	{
		m_ParticleModule.SpawnBurstCount = SpawnBurstCount;
	}

	ImGui::Text("SpawnBurstRepeatTime");
	ImGui::SameLine(100);
	ImGui::DragFloat("##SpawnBurstRepeatTime", &m_ParticleModule.SpawnBurstRepeatTime);

	ImGui::Text("");
	ImGui::Separator();

	// Scale Module
	ImGui::Text("Scale Module");

	ImGui::Separator();

	ImGui::Text("Active");
	ImGui::SameLine(100);
	bool ScaleModuleActive = m_ParticleModule.Module[(UINT)PARTICLE_MODULE::SCALE];
	if (ImGui::Checkbox("##ScaleModuleActive", &ScaleModuleActive))
	{
		m_ParticleModule.Module[(UINT)PARTICLE_MODULE::SCALE] = ScaleModuleActive;
	}

	ImGui::Text("StartScale");
	ImGui::SameLine(100);
	ImGui::DragFloat("##StartScale", &m_ParticleModule.StartScale);

	ImGui::Text("EndScale");
	ImGui::SameLine(100);
	ImGui::DragFloat("##EndScale", &m_ParticleModule.EndScale);

	ImGui::Text("");
	ImGui::Separator();

	// AddVelocity Module
	ImGui::Text("AddVelocity Module");

	ImGui::Separator();

	ImGui::Text("Active");
	ImGui::SameLine(100);
	bool AddVelocityModuleActive = m_ParticleModule.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
	if (ImGui::Checkbox("##AddVelocityModuleActive", &AddVelocityModuleActive))
	{
		m_ParticleModule.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = AddVelocityModuleActive;
	}

	ImGui::Text("AddVelocityType");
	int AddVelocityType = m_ParticleModule.AddVelocityType;
	if (ImGui::RadioButton("Random", AddVelocityType == 0))
	{
		AddVelocityType = 0;
	}
	if (ImGui::RadioButton("FromCenter", AddVelocityType == 1))
	{
		AddVelocityType = 1;
	}
	if (ImGui::RadioButton("ToCenter", AddVelocityType == 2))
	{
		AddVelocityType = 2;
	}
	if (ImGui::RadioButton("Fixed", AddVelocityType == 3))
	{
		AddVelocityType = 3;
	}
	m_ParticleModule.AddVelocityType = AddVelocityType;

	ImGui::Text("AddVelocityFixedDir");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##AddVelocityFixedDir", m_ParticleModule.AddVelocityFixedDir);

	ImGui::Text("AddMinSpeed");
	ImGui::SameLine(100);
	ImGui::DragFloat("##AddMinSpeed", &m_ParticleModule.AddMinSpeed);

	ImGui::Text("AddMaxSpeed");
	ImGui::SameLine(100);
	ImGui::DragFloat("##AddMaxSpeed", &m_ParticleModule.AddMaxSpeed);

	ImGui::Text("");
	ImGui::Separator();

	// Drag Module (감속)
	ImGui::Text("Drag Module(Deceleration)");

	ImGui::Separator();

	ImGui::Text("Active");
	ImGui::SameLine(100);
	bool DragModuleActive = m_ParticleModule.Module[(UINT)PARTICLE_MODULE::DRAG];
	if (ImGui::Checkbox("##DragModuleActive", &DragModuleActive))
	{
		m_ParticleModule.Module[(UINT)PARTICLE_MODULE::DRAG] = DragModuleActive;
	}

	ImGui::Text("DestNormalizedAge");
	ImGui::SameLine(100);
	ImGui::DragFloat("##DestNormalizedAge", &m_ParticleModule.DestNormalizedAge);

	ImGui::Text("LimitSpeed");
	ImGui::SameLine(100);
	ImGui::DragFloat("##LimitSpeed", &m_ParticleModule.LimitSpeed);

	ImGui::Text("");
	ImGui::Separator();

	// Noise Force Module
	ImGui::Text("Noise Force Module");

	ImGui::Separator();

	ImGui::Text("Active");
	ImGui::SameLine(100);
	bool NoiseForceModuleActive = m_ParticleModule.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE];
	if (ImGui::Checkbox("##NoiseForceModuleActive", &NoiseForceModuleActive))
	{
		m_ParticleModule.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = NoiseForceModuleActive;
	}

	ImGui::Text("NoiseForceTerm");
	ImGui::SameLine(100);
	ImGui::DragFloat("##NoiseForceTerm", &m_ParticleModule.NoiseForceTerm);

	ImGui::Text("NoiseForceScale");
	ImGui::SameLine(100);
	ImGui::DragFloat("##NoiseForceScale", &m_ParticleModule.NoiseForceScale);

	ImGui::Text("");
	ImGui::Separator();

	// Render Module
	ImGui::Text("Render Module");

	ImGui::Separator();

	ImGui::Text("Active");
	ImGui::SameLine(100);
	bool RenderModuleActive = m_ParticleModule.Module[(UINT)PARTICLE_MODULE::RENDER];
	if (ImGui::Checkbox("##RenderModuleActive", &RenderModuleActive))
	{
		m_ParticleModule.Module[(UINT)PARTICLE_MODULE::RENDER] = RenderModuleActive;
	}

	ImGui::Text("EndColor");
	ImGui::SameLine(100);
	Vec3 EndColor = m_ParticleModule.EndColor;
	ImGui::ColorEdit3("##EndColor", EndColor);
	m_ParticleModule.EndColor = EndColor;

	ImGui::Text("FadeOut");
	ImGui::SameLine(100);
	bool FadeOut = m_ParticleModule.FadeOut;
	if (ImGui::Checkbox("##FadeOut", &FadeOut))
	{
		m_ParticleModule.FadeOut = FadeOut;
	}

	ImGui::Text("FadeOutStartRatio");
	ImGui::SameLine(100);
	ImGui::DragFloat("##FadeOutStartRatio", &m_ParticleModule.FadeOutStartRatio);

	ImGui::Text("VelocityAlignment");
	ImGui::SameLine(100);
	bool VelocityAlignment = m_ParticleModule.VelocityAlignment;
	if (ImGui::Checkbox("##VelocityAlignment", &VelocityAlignment))
	{
		m_ParticleModule.VelocityAlignment = VelocityAlignment;
	}

	m_Particle->SetParticleModule(m_ParticleModule);
}

void ParticleSystemUI::SaveLoad()
{
	if (ImGui::Button("Save##Particle", ImVec2(50.f, 18.f)))
	{
		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"particle\0*.particle";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"particle\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetSaveFileName(&ofn))
		{
			FILE* File = nullptr;
			_wfopen_s(&File, szSelect, L"wb");

			if (nullptr == File)
				return;

			m_Particle->SaveToFile(File);

			fclose(File);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Load##Particle", ImVec2(50.f, 18.f)))
	{
		wchar_t szSelect[256] = {};

		OPENFILENAME ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szSelect;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szSelect);
		ofn.lpstrFilter = L"particle\0*.particle";
		ofn.nFilterIndex = 0;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		// 탐색창 초기 위치 지정
		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"particle\\";
		ofn.lpstrInitialDir = strInitPath.c_str();

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			FILE* File = nullptr;
			_wfopen_s(&File, szSelect, L"rb");

			if (nullptr == File)
				return;

			m_Particle->LoadFromFile(File);

			fclose(File);
		}
		else
		{
			return;
		}

		m_ParticleModule = m_Particle->GetParticleModule();
		m_ParticleTex = m_Particle->GetParticleTexture();
	}
}

void ParticleSystemUI::SelectTexture(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_ParticleTex = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	SetParticleTex(pTex);
	m_Particle->SetParticleTexture(m_ParticleTex);
}

void ParticleSystemUI::SetParticleTex(Ptr<CTexture> _Tex)
{
	m_ParticleTex = _Tex;
}
