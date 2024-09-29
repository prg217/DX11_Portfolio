#include "pch.h"
#include "CGrassScript.h"

#include "CGlowingPlantsScript.h"
#include "CCountDownDeleteScript.h"

#include "CSwingObjScript.h"

#include <random>

CGrassScript::CGrassScript()
	: CScript(UINT(SCRIPT_TYPE::GRASSSCRIPT))
	, m_GrassType(GrassType::Glowing_Purple)
	, m_GlowingLight(nullptr)
	, m_GlowingTime(2.f)
	, m_SaveTime(0.f)
	, m_GlowingActive(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "GrassType", &m_GrassType);
}

CGrassScript::CGrassScript(const CGrassScript& _Origin)
	: CScript(_Origin)
	, m_GrassType(_Origin.m_GrassType)
	, m_GlowingLight(nullptr)
	, m_GlowingTime(_Origin.m_GlowingTime)
	, m_SaveTime(0.f)
	, m_GlowingActive(false)
{
}

CGrassScript::~CGrassScript()
{
}

void CGrassScript::Begin()
{
	// 빛
	m_GlowingLight = new CGameObject;
	m_GlowingLight->SetName(L"GlowingPointLight");
	m_GlowingLight->AddComponent(new CTransform);
	m_GlowingLight->AddComponent(new CLight2D);

	m_GlowingLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	m_GlowingLight->Light2D()->SetLightColor(Vec3(0.6f, 0.6f, 0.7f));
	m_GlowingLight->Light2D()->SetRadius(0.f);
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	m_GlowingLight->Transform()->SetRelativePos(pos);

	CreateObject(m_GlowingLight, 0);

	switch (m_GrassType)
	{
	case GrassType::Glowing_Purple:
		GlowingPlants();
		break;
	default:
		break;
	}
}

void CGrassScript::Tick()
{
	if ((TIME - m_SaveTime >= m_SaveTime))
	{
		m_GlowingActive = false;
		m_GlowingLight->Light2D()->SetRadius(0.f);

		for (auto i : GetOwner()->GetChildren())
		{
			CScript* script = i->GetScript("CGlowingPlantsScript");
			CGlowingPlantsScript* GPscript = dynamic_cast<CGlowingPlantsScript*>(script);
			if (GPscript)
			{
				GPscript->GlowingEnd();
			}
		}
	}
}

void CGrassScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 플레이어와 직접적으로 부딪쳤을 때
	if (_OtherObject->GetLayerIdx() == 3)
	{
		std::random_device rd;  // 하드웨어 기반 난수 생성기
		std::mt19937 gen(rd()); // Mersenne Twister 난수 생성기
		std::uniform_int_distribution<int> dist(0, 2); // 0부터 2까지의 범위에서 균등 분포
		int random = dist(gen); // 랜덤 값 반환

		switch (random)
		{
		case 0:
		{
			Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_152_GlowingReedTouch_1.wav");
			pSound->Play(1, 1.f, true);
		}
		break;
		case 1:
		{
			Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_153_GlowingReedTouch_2.wav");
			pSound->Play(1, 1.f, true);
		}
		break;
		case 3:
		{
			Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_154_GlowingReedTouch_3.wav");
			pSound->Play(1, 1.f, true);
		}
		break;
		default:
			break;
		}

		m_SaveTime = TIME;
		m_GlowingActive = true;
		for (auto i : GetOwner()->GetChildren())
		{
			CScript* script = i->GetScript("CGlowingPlantsScript");
			CGlowingPlantsScript* GPscript = dynamic_cast<CGlowingPlantsScript*>(script);
			if (GPscript)
			{
				GPscript->MoveStart();
			}
		}
		// 빛 활성화
		m_GlowingLight->Light2D()->SetRadius(50.f);
		// 파티클
		CGameObject* glowingParticle = new CGameObject;
		glowingParticle->SetName(L"GlowingParticle");
		glowingParticle->AddComponent(new CTransform);
		glowingParticle->AddComponent(new CParticleSystem);

		glowingParticle->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
		glowingParticle->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 0.f));

		wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
		strInitPath += L"particle\\glowingPlant.particle";

		FILE* File = nullptr;
		_wfopen_s(&File, strInitPath.c_str(), L"rb");

		glowingParticle->ParticleSystem()->LoadFromFile(File);
		fclose(File);

		// 삭제되는 시간을 지정해준다.
		glowingParticle->AddComponent(new CCountDownDeleteScript);
		CScript* pScript = glowingParticle->GetScript("CCountDownDeleteScript");
		CCountDownDeleteScript* pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
		pCountDown->SetSaveTime(TIME);
		pCountDown->SetDeadTime(m_GlowingTime);

		CreateObject(glowingParticle, 0);
	}
}

void CGrassScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 플레이어가 채 휘두르기 중일 때 채 휘두르기 콜라이더랑 만나면 삭제
	if (_OtherObject->GetLayerIdx() == 7)
	{
		CScript* script = _OtherObject->GetScript("CSwingObjScript");
		CSwingObjScript* pSwing = dynamic_cast<CSwingObjScript*>(script);
		if (pSwing)
		{
			if (pSwing->GetIsSwing())
			{
				DeleteObject(m_GlowingLight);
				DeleteObject(GetOwner());

				// 풀 흩어지는 효과 파티클
				CGameObject* glowingParticle = new CGameObject;
				glowingParticle->SetName(L"GlowingDestroyParticle");
				glowingParticle->AddComponent(new CTransform);
				glowingParticle->AddComponent(new CParticleSystem);

				glowingParticle->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
				glowingParticle->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 0.f));

				wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
				strInitPath += L"particle\\glowingPlant_destroy.particle";

				FILE* File = nullptr;
				_wfopen_s(&File, strInitPath.c_str(), L"rb");

				glowingParticle->ParticleSystem()->LoadFromFile(File);
				fclose(File);

				// 삭제되는 시간을 지정해준다.
				glowingParticle->AddComponent(new CCountDownDeleteScript);
				CScript* pScript = glowingParticle->GetScript("CCountDownDeleteScript");
				CCountDownDeleteScript* pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
				pCountDown->SetSaveTime(TIME);
				pCountDown->SetDeadTime(3.f);

				CreateObject(glowingParticle, 0);
			}
		}
	}
}

void CGrassScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	
}

void CGrassScript::SaveToFile(FILE* _File)
{
	fwrite(&m_GrassType, sizeof(GrassType), 1, _File);
}

void CGrassScript::LoadFromFile(FILE* _File)
{
	fread(&m_GrassType, sizeof(GrassType), 1, _File);
}

void CGrassScript::GlowingPlants()
{
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");
	Ptr<CFlipBook> pFlipBook;
	CGameObject* pGlowingPlant = new CGameObject;
	pGlowingPlant->SetName(L"GlowingPlants_Purple0");
	pGlowingPlant->AddComponent(new CTransform);
	pGlowingPlant->AddComponent(new CMeshRender);
	pGlowingPlant->AddComponent(new CFlipBookComponent);
	pGlowingPlant->AddComponent(new CGlowingPlantsScript);

	pGlowingPlant->Transform()->SetRelativePos(0.f, -0.2f, 0.f);
	pGlowingPlant->Transform()->SetRelativeScale(1.f, 1.f, 0.f);

	pGlowingPlant->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pGlowingPlant->MeshRender()->SetMaterial(pMtrl);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_purple_0.flip");
	pGlowingPlant->FlipBookComponent()->AddFlipBook(0, pFlipBook);
	pGlowingPlant->FlipBookComponent()->Play(0, 0, false);

	CScript* script = pGlowingPlant->GetScript("CGlowingPlantsScript");
	CGlowingPlantsScript* GPscript = dynamic_cast<CGlowingPlantsScript*>(script);
	GPscript->SetNumber(0);

	CreateObject(pGlowingPlant, 0);
	AddChildObject(GetOwner(), pGlowingPlant);

	pGlowingPlant = new CGameObject;
	pGlowingPlant->SetName(L"GlowingPlants_Purple1");
	pGlowingPlant->AddComponent(new CTransform);
	pGlowingPlant->AddComponent(new CMeshRender);
	pGlowingPlant->AddComponent(new CFlipBookComponent);
	pGlowingPlant->AddComponent(new CGlowingPlantsScript);

	pGlowingPlant->Transform()->SetRelativePos(-0.15f, -0.16f, 0.f);
	pGlowingPlant->Transform()->SetRelativeScale(1.f, 1.f, 0.f);

	pGlowingPlant->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pGlowingPlant->MeshRender()->SetMaterial(pMtrl);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_purple_1.flip");
	pGlowingPlant->FlipBookComponent()->AddFlipBook(0, pFlipBook);
	pGlowingPlant->FlipBookComponent()->Play(0, 0, false);

	script = pGlowingPlant->GetScript("CGlowingPlantsScript");
	GPscript = dynamic_cast<CGlowingPlantsScript*>(script);
	GPscript->SetNumber(1);

	CreateObject(pGlowingPlant, 0);
	AddChildObject(GetOwner(), pGlowingPlant);

	pGlowingPlant = new CGameObject;
	pGlowingPlant->SetName(L"GlowingPlants_Purple2");
	pGlowingPlant->AddComponent(new CTransform);
	pGlowingPlant->AddComponent(new CMeshRender);
	pGlowingPlant->AddComponent(new CFlipBookComponent);
	pGlowingPlant->AddComponent(new CGlowingPlantsScript);

	pGlowingPlant->Transform()->SetRelativePos(0.15f, -0.16f, 0.f);
	pGlowingPlant->Transform()->SetRelativeScale(1.f, 1.f, 0.f);

	pGlowingPlant->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pGlowingPlant->MeshRender()->SetMaterial(pMtrl);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_purple_2.flip");
	pGlowingPlant->FlipBookComponent()->AddFlipBook(0, pFlipBook);
	pGlowingPlant->FlipBookComponent()->Play(0, 0, false);

	script = pGlowingPlant->GetScript("CGlowingPlantsScript");
	GPscript = dynamic_cast<CGlowingPlantsScript*>(script);
	GPscript->SetNumber(2);

	CreateObject(pGlowingPlant, 0);
	AddChildObject(GetOwner(), pGlowingPlant);

	pGlowingPlant = new CGameObject;
	pGlowingPlant->SetName(L"GlowingPlants_Purple3");
	pGlowingPlant->AddComponent(new CTransform);
	pGlowingPlant->AddComponent(new CMeshRender);
	pGlowingPlant->AddComponent(new CFlipBookComponent);
	pGlowingPlant->AddComponent(new CGlowingPlantsScript);

	pGlowingPlant->Transform()->SetRelativePos(-0.1f, -0.1f, -1.f);
	pGlowingPlant->Transform()->SetRelativeScale(1.f, 1.f, 0.f);

	pGlowingPlant->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pGlowingPlant->MeshRender()->SetMaterial(pMtrl);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_purple_3.flip");
	pGlowingPlant->FlipBookComponent()->AddFlipBook(0, pFlipBook);
	pGlowingPlant->FlipBookComponent()->Play(0, 0, false);

	script = pGlowingPlant->GetScript("CGlowingPlantsScript");
	GPscript = dynamic_cast<CGlowingPlantsScript*>(script);
	GPscript->SetNumber(3);

	CreateObject(pGlowingPlant, 0);
	AddChildObject(GetOwner(), pGlowingPlant);

	pGlowingPlant = new CGameObject;
	pGlowingPlant->SetName(L"GlowingPlants_Purple4");
	pGlowingPlant->AddComponent(new CTransform);
	pGlowingPlant->AddComponent(new CMeshRender);
	pGlowingPlant->AddComponent(new CFlipBookComponent);
	pGlowingPlant->AddComponent(new CGlowingPlantsScript);

	pGlowingPlant->Transform()->SetRelativePos(0.05f, -0.12f, -1.f);
	pGlowingPlant->Transform()->SetRelativeScale(1.f, 1.f, 0.f);

	pGlowingPlant->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pGlowingPlant->MeshRender()->SetMaterial(pMtrl);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obj\\GlowingPlants\\obj_glowingPlant_purple_4.flip");
	pGlowingPlant->FlipBookComponent()->AddFlipBook(0, pFlipBook);
	pGlowingPlant->FlipBookComponent()->Play(0, 0, false);

	script = pGlowingPlant->GetScript("CGlowingPlantsScript");
	GPscript = dynamic_cast<CGlowingPlantsScript*>(script);
	GPscript->SetNumber(4);

	CreateObject(pGlowingPlant, 0);
	AddChildObject(GetOwner(), pGlowingPlant);
}
