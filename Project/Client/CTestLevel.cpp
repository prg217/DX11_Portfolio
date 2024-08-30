#include "pch.h"
#include "CTestLevel.h"

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/enum.h>

#include <Engine/CCollisionMgr.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CCameraMoveScript.h>

#include <Engine/CSetColorCS.h>
#include <Engine/CStructuredBuffer.h>

#include "CLevelSaveLoad.h"

void CTestLevel::CreateTestLevel()
{
	// Material
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");
	Ptr<CMaterial> pAlphaBlendMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	Ptr<CMaterial> pDebugShapeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");

	Ptr<CTexture> pTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture//Character.png");
	pAlphaBlendMtrl->SetTexParam(TEX_0, pTexture);

	CreatePrefab();

	// Sound 재생
	//Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\DM.wav");
	//pSound->Play(0, 1.f, false);


	// Level 생성
	CLevel* pLevel = new CLevel;
	
	pLevel->GetLayer(0)->SetName(L"Default");
	pLevel->GetLayer(1)->SetName(L"Background");
	pLevel->GetLayer(2)->SetName(L"Tile");
	pLevel->GetLayer(3)->SetName(L"Player");
	pLevel->GetLayer(4)->SetName(L"Monster");
	pLevel->GetLayer(5)->SetName(L"PlayerProjectile");
	pLevel->GetLayer(6)->SetName(L"MonsterProjectile");

	// 카메라 오브젝트
	CGameObject* CamObj = new CGameObject;
	CamObj->SetName(L"MainCamera");
	CamObj->AddComponent(new CTransform);
	CamObj->AddComponent(new CCamera);
	//CamObj->AddComponent(new CCameraMoveScript);

	// 우선순위를 0 : MainCamera 로 설정
	CamObj->Camera()->SetPriority(0);

	// 카메라 레이어 설정 (31번 레이어 제외 모든 레이어를 촬영)
	CamObj->Camera()->SetLayerAll();
	CamObj->Camera()->SetLayer(31, false);
	CamObj->Camera()->SetFar(100000.f);
	CamObj->Camera()->SetProjType(ORTHOGRAPHIC);

	pLevel->AddObject(0, CamObj);

	CGameObject* pObject = nullptr;


	// 광원 오브젝트 추가
	pObject = new CGameObject;
	pObject->SetName(L"PointLight 1");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light2D()->SetRadius(1000.f);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	pLevel->AddObject(0, pObject);
		

	// 플레이어 오브젝트
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	//pPlayer->AddComponent(new CRigidBody);
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->AddComponent(new CFlipBookComponent);
	pPlayer->AddComponent(new CPlayerScript);
	pPlayer->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pPlayer->Transform()->SetRelativeScale(150.f, 150.f, 1.f);
	
	pPlayer->Collider2D()->SetIndependentScale(false);
	pPlayer->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayer->Collider2D()->SetScale(Vec3(1.f, 1.f, 1.f));
	//pPlayer->RigidBody()->UseGravity(true);
	
	pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pPlayer->MeshRender()->SetMaterial(pMtrl);
	
	Ptr<CFlipBook> pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle.flip");
	pPlayer->FlipBookComponent()->AddFlipBook(0, pFlipBook);
	pPlayer->FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE, 7, true);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle_back.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_IDLE_BACK, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_IDLE_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_IDLE_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_IDLE_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_IDLE_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_IDLE_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_IDLE_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_walk_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_WALK_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_walk_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_WALK_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_walk_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_WALK_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_walk_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_WALK_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_walk_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_WALK_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_walk_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_WALK_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_walk_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_WALK_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_walk_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_WALK_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_run_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_RUN_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_run_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_RUN_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_run_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_RUN_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_run_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_RUN_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_run_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_RUN_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_run_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_RUN_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_run_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_RUN_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_run_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_RUN_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_idle_dance.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_IDLE_DANCE, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_dance.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_DANCE, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_swing_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_SWING_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_swing_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_SWING_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_swing_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_SWING_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_swing_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_SWING_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_swing_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_SWING_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_swing_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_SWING_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_swing_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_SWING_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\ogu_swing_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)FLIPBOOK_IDX::OGU_SWING_RIGHTUP, pFlipBook);

	pLevel->AddObject(3, pPlayer);


	// Monster Object
	//CGameObject* pMonster = new CGameObject;
	//pMonster->SetName(L"Monster");

	//pMonster->AddComponent(new CTransform);
	//pMonster->AddComponent(new CMeshRender);
	//pMonster->AddComponent(new CCollider2D);

	//pMonster->Transform()->SetRelativePos(-400.f, 0.f, 100.f);
	//pMonster->Transform()->SetRelativeScale(150.f, 150.f, 1.f);	

	//pMonster->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	//pMonster->Collider2D()->SetScale(Vec3(1.2f, 1.2f, 1.f));

	//pMonster->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pMonster->MeshRender()->SetMaterial(pMtrl);

	//pLevel->AddObject(4, pMonster);

	// TileMap Object
	CGameObject* pTileMapObj = new CGameObject;
	pTileMapObj->SetName(L"TileMap");
	
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);
	
	pTileMapObj->Transform()->SetRelativePos(Vec3(-500.f, 250.f, 500.f));

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"tile\\test1.tile";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");
		
	pTileMapObj->TileMap()->LoadFromFile(File);
	fclose(File);
	pLevel->AddObject(2, pTileMapObj);


	// Particle Object
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"RunParticle");
	
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);
	
	strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"particle\\ogu_run.particle";
	
	File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");
	
	pParticleObj->ParticleSystem()->LoadFromFile(File);
	fclose(File);
	
	pLevel->AddObject(0, pParticleObj);


	// PostProcess Object
	//CGameObject* pGrayFilterObj = new CGameObject;
	//pGrayFilterObj->SetName(L"GrayFilter");
	//pGrayFilterObj->AddComponent(new CTransform);
	//pGrayFilterObj->AddComponent(new CMeshRender);

	//pGrayFilterObj->Transform()->SetRelativeScale(150.f, 150.f, 1.f);

	//pGrayFilterObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pGrayFilterObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DistortionMtrl"));

	//pLevel->AddObject(0, pGrayFilterObj);

	ChangeLevel(pLevel, LEVEL_STATE::STOP);

	// 충돌 지정
	CCollisionMgr::GetInst()->CollisionCheck(3, 4); // Player vs Monster
	CCollisionMgr::GetInst()->CollisionCheck(5, 4); // Player Projectile vs Monster	
}

void CTestLevel::CreatePrefab()
{
	/*CGameObject* pProto = new CGameObject;
	pProto->SetName("Missile");
	pProto->AddComponent(new CTransform);
	pProto->AddComponent(new CMeshRender);
	pProto->AddComponent(new CMissileScript);

	pProto->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pProto->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pProto->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetProtoObject(pProto);

	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePref", pPrefab);

	wstring FilePath = CPathMgr::GetInst()->GetContentPath();
	FilePath += L"prefab\\Missile.pref";
	pPrefab->Save(FilePath);*/
}
