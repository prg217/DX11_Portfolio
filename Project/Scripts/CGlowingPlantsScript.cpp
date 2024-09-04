#include "pch.h"
#include "CGlowingPlantsScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CGlowingPlantsScript::CGlowingPlantsScript()
	: CScript(UINT(SCRIPT_TYPE::GLOWINGPLANTSSCRIPT))
	, m_Number(0)
	, m_MoveStart(false)
	, m_ZRotation(0)
	, m_SaveTime(0)
	, m_ShakingTime(1.f)
	, m_GlowingObj(nullptr)
{
}

CGlowingPlantsScript::CGlowingPlantsScript(const CGlowingPlantsScript& _Origin)
	: CScript(_Origin)
	, m_Number(_Origin.m_Number)
	, m_MoveStart(false)
	, m_ZRotation(0)
	, m_SaveTime(0)
	, m_ShakingTime(_Origin.m_ShakingTime)
	, m_GlowingObj(nullptr)
{
}

CGlowingPlantsScript::~CGlowingPlantsScript()
{
}

void CGlowingPlantsScript::Begin()
{
}

void CGlowingPlantsScript::Tick()
{
	if (m_MoveStart)
	{
		if (TIME - m_SaveTime >= m_ShakingTime)
		{
			m_ZRotation = 0.f;
			GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
			m_MoveStart = false;
			return;
		}

		float shaking = 1.f; // 몇 번 흔들건지
		float amplitude = 0.1f; // 최대 각도
		switch (m_Number)
		{
		case 0:
			shaking = -1.f;
			amplitude = 0.13f;
			m_ZRotation = amplitude * sin(shaking * 2.0f * 3.14159f * TIME);
			GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, m_ZRotation));
			break;
		case 1:
			shaking = -1.2f;
			amplitude = 0.1f;
			m_ZRotation = amplitude * sin(shaking * 2.0f * 3.14159f * TIME);
			GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, m_ZRotation));
			break;
		case 2:
			shaking = 1.f;
			amplitude = 0.12f;
			m_ZRotation = amplitude * sin(shaking * 2.0f * 3.14159f * TIME);
			GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, m_ZRotation));
			break;
		case 3:
			shaking = -0.9f;
			amplitude = 0.15f;
			m_ZRotation = amplitude * sin(shaking * 2.0f * 3.14159f * TIME);
			GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, m_ZRotation));
			break;
		case 4:
			shaking = 1.f;
			m_ZRotation = amplitude * sin(shaking * 2.0f * 3.14159f * TIME);
			GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, m_ZRotation));
			break;
		default:
			break;
		}
	}
}

void CGlowingPlantsScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Number, sizeof(int), 1, _File);
}

void CGlowingPlantsScript::LoadFromFile(FILE* _File)
{
	fread(&m_Number, sizeof(int), 1, _File);
}

void CGlowingPlantsScript::SetNumber(int _Num)
{
	m_Number = _Num;

	Ptr<CFlipBook> pFlipBook;
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	m_GlowingObj = new CGameObject;
	m_GlowingObj->SetName(L"Glowing");
	m_GlowingObj->AddComponent(new CTransform);
	m_GlowingObj->AddComponent(new CMeshRender);
	m_GlowingObj->AddComponent(new CFlipBookComponent);

	m_GlowingObj->Transform()->SetRelativePos(0.f, 0.05f, 0.f);
	m_GlowingObj->Transform()->SetRelativeScale(0.f, 0.f, 0.f);

	m_GlowingObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_GlowingObj->MeshRender()->SetMaterial(pMtrl);

	switch (m_Number)
	{
	case 0:
		pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_glowing_0.flip");
		break;
	case 1:
		pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_glowing_1.flip");
		break;
	case 2:
		pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_glowing_2.flip");
		break;
	case 3:
		pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_glowing_3.flip");
		break;
	case 4:
		pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_glowing_4.flip");
		break;
	default:
		break;
	}
	m_GlowingObj->FlipBookComponent()->AddFlipBook(0, pFlipBook);
	m_GlowingObj->FlipBookComponent()->Play(0, 0, false);

	CreateObject(m_GlowingObj, 0);
	AddChildObject(GetOwner(), m_GlowingObj);
}

void CGlowingPlantsScript::MoveStart()
{
	m_MoveStart = true;
	m_SaveTime = TIME;

	m_GlowingObj->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
}

void CGlowingPlantsScript::GlowingEnd()
{
	m_GlowingObj->Transform()->SetRelativeScale(0.f, 0.f, 0.f);
}
