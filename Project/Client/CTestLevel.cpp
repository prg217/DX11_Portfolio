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
#include <Scripts/CCameraPlayerTrackingScript.h>
#include <Scripts/CSwingObjScript.h>
#include <Scripts/CPlayerInteractionScript.h>
#include <Scripts/CInteractionScript.h>
#include <Scripts/CLiftScript.h>
#include <Scripts/CPushScript.h>
#include <Scripts/CGrassScript.h>
#include <Scripts/CNPCScript.h>
#include <Scripts/CPlayerCameraScript.h>

#include <Engine/CSetColorCS.h>
#include <Engine/CStructuredBuffer.h>

#include "CLevelSaveLoad.h"

void CTestLevel::CreateTestLevel()
{
	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"level\\boss.lv";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(strInitPath);
	fclose(File);
	ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);
	
	// Material
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");
	Ptr<CMaterial> pAlphaBlendMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	Ptr<CMaterial> pDebugShapeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");

	Ptr<CTexture> pTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture//Character.png");
	pAlphaBlendMtrl->SetTexParam(TEX_0, pTexture);

	// Sound ���
	//Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\DM.wav");
	//pSound->Play(0, 1.f, false);


	// Level ����
	//CLevel* pLevel = new CLevel;
	
	pLoadedLevel->GetLayer(0)->SetName(L"Default");
	pLoadedLevel->GetLayer(1)->SetName(L"Background");
	pLoadedLevel->GetLayer(2)->SetName(L"Tile");
	pLoadedLevel->GetLayer(3)->SetName(L"Player");
	pLoadedLevel->GetLayer(4)->SetName(L"Monster");
	pLoadedLevel->GetLayer(5)->SetName(L"PlayerInteraction");
	pLoadedLevel->GetLayer(6)->SetName(L"Interaction");
	pLoadedLevel->GetLayer(7)->SetName(L"PlayerSwing");
	pLoadedLevel->GetLayer(8)->SetName(L"Detect");
	pLoadedLevel->GetLayer(9)->SetName(L"DestroyBlock");
	pLoadedLevel->GetLayer(10)->SetName(L"MonsterAttack");

	pLoadedLevel->GetLayer(28)->SetName(L"Block");
	pLoadedLevel->GetLayer(29)->SetName(L"CameraBlock");
	pLoadedLevel->GetLayer(30)->SetName(L"Camera");
	pLoadedLevel->GetLayer(31)->SetName(L"UI");

	// TileMap Object
	//CGameObject* pTileMapObj = new CGameObject;
	//pTileMapObj->SetName(L"TileMap");
	//
	//pTileMapObj->AddComponent(new CTransform);
	//pTileMapObj->AddComponent(new CTileMap);
	//
	//pTileMapObj->Transform()->SetRelativePos(Vec3(-500.f, 250.f, 500.f));
	//
	//strInitPath = CPathMgr::GetInst()->GetContentPath();
	//strInitPath += L"tile\\boss.tile";
	//
	//File = nullptr;
	//_wfopen_s(&File, strInitPath.c_str(), L"rb");
	//
	//pTileMapObj->TileMap()->LoadFromFile(File);
	//fclose(File);
	//pLoadedLevel->AddObject(2, pTileMapObj);

	// Effect Object
	//CGameObject* pMonster = new CGameObject;
	//pMonster->SetName(L"Area_White");
	//
	//pMonster->AddComponent(new CTransform);
	//pMonster->AddComponent(new CMeshRender);
	//
	//pMonster->Transform()->SetRelativePos(0.f, 0.f, -1.f);
	//pMonster->Transform()->SetRelativeScale(1.f, 1.f, 1.f);
	//
	//pMonster->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pMonster->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffectMtrl"));
	//
	//pMonster->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(10.f, 10.f, 10.f, 1.f));
	//pMonster->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\obstacle\\Sprite_MoonForest_FlowerLightStandArea.png"));
	//
	//pLoadedLevel->AddObject(0, pMonster);

	//CGameObject* pObj = new CGameObject;
	//pObj->SetName(L"BugBall");
	//pObj->AddComponent(new CTransform);
	//pObj->AddComponent(new CCollider2D);
	//pObj->AddComponent(new CMeshRender);
	//pObj->AddComponent(new CFlipBookComponent);
	//pObj->AddComponent(new CLight2D);
	
	//pObj->Transform()->SetRelativePos(Vec3(0, 0, 0));
	//pObj->Transform()->SetRelativeScale(Vec3(150, 150, 1));
	//
	//pObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pObj->MeshRender()->SetMaterial(pAlphaBlendMtrl);
	//
	//pObj->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	//pObj->Collider2D()->SetScale(Vec3(0.15f, 0.15f, 1.f));
	//
	//Ptr<CFlipBook> pFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Boss\\Ball\\ball.flip");
	//pObj->FlipBookComponent()->AddFlipBook(0, pFlip);
	//pObj->FlipBookComponent()->Play(0, 8, true);
	//pObj->FlipBookComponent()->AddAlpha(0.5f);
	//pObj->FlipBookComponent()->SetUseLight(false);

	//Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\boss\\ball\\lightBall_W.sprite");
	//pObj->SpriteComponent()->AddSprite(pSprite);
	//pObj->SpriteComponent()->AddColor(false, Vec3(0, 0, 0));

	//pObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pObj->Light2D()->SetRadius(10.f);

	//strInitPath = CPathMgr::GetInst()->GetContentPath();
	//strInitPath += L"particle\\monster_poison.particle";
	//
	//File = nullptr;
	//_wfopen_s(&File, strInitPath.c_str(), L"rb");
	//
	//pObj->ParticleSystem()->LoadFromFile(File);
	//fclose(File);

	//pLoadedLevel->AddObject(10, pObj);
/*
	// ī�޶� ������Ʈ
	CGameObject* CamObj = new CGameObject;
	CamObj->SetName(L"MainCamera");
	CamObj->AddComponent(new CTransform);
	CamObj->AddComponent(new CCamera);
	CamObj->AddComponent(new CCameraPlayerTrackingScript);
	// �÷��̾�� ī�޶� ���̾ ���� ������Ʈ(�ݶ��̴� ����)�� ������ �ִٰ� �°� �������� �Ǹ� ��ȣ ������ ī�޶� Ư�� ��... ���߰�...

	// �켱������ 0 : MainCamera �� ����
	CamObj->Camera()->SetPriority(0);

	// ī�޶� ���̾� ���� (8, 31�� ���̾� ���� ��� ���̾ �Կ�)
	CamObj->Camera()->SetLayerAll();
	CamObj->Camera()->SetLayer(31, false);
	CamObj->Camera()->SetFar(100000.f);
	CamObj->Camera()->SetProjType(ORTHOGRAPHIC);

	pLevel->AddObject(30, CamObj);

	// UI ī�޶� ������Ʈ
	CamObj = new CGameObject;
	CamObj->SetName(L"UICamera");
	CamObj->AddComponent(new CTransform);
	CamObj->AddComponent(new CCamera);

	// �켱������ 1
	CamObj->Camera()->SetPriority(1);

	// ī�޶� ���̾� ���� (31�� ���̾ �Կ�)
	CamObj->Camera()->SetLayer(31, true);
	CamObj->Camera()->SetFar(100000.f);
	CamObj->Camera()->SetProjType(ORTHOGRAPHIC);

	pLevel->AddObject(0, CamObj);

	CGameObject* pObject = nullptr;


	// ���� ������Ʈ �߰�
	pObject = new CGameObject;
	pObject->SetName(L"DirectionalLight");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pObject->Light2D()->SetLightColor(Vec3(0.67f, 0.67f, 0.76f));
	pObject->Light2D()->SetRadius(1000.f);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	pLevel->AddObject(0, pObject);
		

	// ===�÷��̾� ������Ʈ===
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddComponent(new CRigidBody);
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->AddComponent(new CFlipBookComponent);
	pPlayer->AddComponent(new CPlayerScript);
	pPlayer->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pPlayer->Transform()->SetRelativeScale(150.f, 150.f, 1.f);
	
	pPlayer->Collider2D()->SetIndependentScale(false);
	pPlayer->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayer->Collider2D()->SetScale(Vec3(0.3f, 0.3f, 1.f));

	pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pPlayer->MeshRender()->SetMaterial(pMtrl);
	
	// �÷��̾� �ִϸ��̼� ���
	PlayerAni(pPlayer);

	// �÷��̾ ä �ֵθ��⸦ �� �� ���� ������Ʈ
	CGameObject* pSwingObj = new CGameObject;
	pSwingObj->SetName(L"PlayerSwingObj");
	pSwingObj->AddComponent(new CTransform);
	pSwingObj->AddComponent(new CMeshRender);
	pSwingObj->AddComponent(new CCollider2D);
	pSwingObj->AddComponent(new CFlipBookComponent);
	pSwingObj->AddComponent(new CSwingObjScript);

	pSwingObj->Transform()->SetRelativePos(0.f, 0.5f, 0.f);
	pSwingObj->Transform()->SetRelativeScale(1.f, 1.f, 0.f);

	pSwingObj->Collider2D()->SetIndependentScale(false);
	pSwingObj->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pSwingObj->Collider2D()->SetScale(Vec3(1.f, 0.3f, 0.3f));

	pSwingObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pSwingObj->MeshRender()->SetMaterial(pMtrl);

	Ptr<CFlipBook> pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\swing_effect.flip");
	pSwingObj->FlipBookComponent()->AddFlipBook(0, pFlipBook);

	// �÷��̾ ��ȣ�ۿ��� �� �� ���� ������Ʈ
	CGameObject* pInteractionObj = new CGameObject;
	pInteractionObj->SetName(L"PlayerInteractionObj");
	pInteractionObj->AddComponent(new CTransform); 
	pInteractionObj->AddComponent(new CCollider2D);
	pInteractionObj->AddComponent(new CPlayerInteractionScript);

	pInteractionObj->Transform()->SetRelativePos(0.f, 0.2f, 0.f);
	pInteractionObj->Transform()->SetRelativeScale(1.f, 1.f, 0.f);

	pInteractionObj->Collider2D()->SetIndependentScale(false);
	pInteractionObj->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pInteractionObj->Collider2D()->SetScale(Vec3(0.5f, 0.3f, 0.5f));

	// �÷��̾� ī�޶� ���� ������Ʈ
	CGameObject* pPlayerCamera = new CGameObject;
	pPlayerCamera->SetName(L"PlayerCamera");
	pPlayerCamera->AddComponent(new CTransform);
	pPlayerCamera->AddComponent(new CCollider2D);
	pPlayerCamera->AddComponent(new CPlayerCameraScript);

	pPlayerCamera->Transform()->SetRelativePos(0.f, 0.f, 0.f);
	pPlayerCamera->Transform()->SetRelativeScale(1.f, 1.f, 0.f);
	pPlayerCamera->Transform()->SetIndependentScale(true);

	pPlayerCamera->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayerCamera->Collider2D()->SetScale(Vec3(200.f, 200.f, 0.0f));

	// ����
	pLevel->AddObject(30, pPlayerCamera);
	pLevel->AddObject(7, pSwingObj);
	pLevel->AddObject(5, pInteractionObj);
	pLevel->AddObject(3, pPlayer);
	// =======================

	// ���ø� �� �ִ� ��
	CGameObject* pLiftStone = new CGameObject;
	pLiftStone->SetName(L"LiftStone");
	pLiftStone->AddComponent(new CTransform);
	pLiftStone->AddComponent(new CMeshRender);
	pLiftStone->AddComponent(new CCollider2D);
	pLiftStone->AddComponent(new CSpriteComponent);
	pLiftStone->AddComponent(new CInteractionScript);
	pLiftStone->AddComponent(new CLiftScript);

	pLiftStone->Transform()->SetRelativePos(100.f, 0.f, 0.f);
	pLiftStone->Transform()->SetRelativeScale(150.f, 150.f, 0.f);
	pLiftStone->Transform()->SetIndependentScale(true);

	pLiftStone->Collider2D()->SetIndependentScale(false);
	pLiftStone->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pLiftStone->Collider2D()->SetScale(Vec3(0.4f, 0.2f, 1.f));

	pLiftStone->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pLiftStone->MeshRender()->SetMaterial(pMtrl);

	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\Obj\\obj_liftStone.sprite");
	pLiftStone->SpriteComponent()->AddSprite(pSprite);

	pLevel->AddObject(6, pLiftStone);


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

	ChangeStopLevel(pLevel, LEVEL_STATE::STOP);
	*/
	// �浹 ����
	CCollisionMgr::GetInst()->CollisionCheck(3, 4); // �÷��̾�, ����
	CCollisionMgr::GetInst()->CollisionCheck(3, 6); // �÷��̾�, ��ȣ�ۿ�
	CCollisionMgr::GetInst()->CollisionCheck(3, 8); // �÷��̾�, ����
	CCollisionMgr::GetInst()->CollisionCheck(4, 4); // ����, ����
	CCollisionMgr::GetInst()->CollisionCheck(4, 7); // ����, �÷��̾� ä �ֵθ���
	CCollisionMgr::GetInst()->CollisionCheck(4, 8); // ����, ����
	CCollisionMgr::GetInst()->CollisionCheck(5, 6); // �÷��̾� ��ȣ�ۿ� ����, ��ȣ�ۿ�
	CCollisionMgr::GetInst()->CollisionCheck(6, 6); // ��ȣ�ۿ�, ��ȣ�ۿ�
	CCollisionMgr::GetInst()->CollisionCheck(7, 6); // �÷��̾� ä �ֵθ���, ��ȣ�ۿ�
	CCollisionMgr::GetInst()->CollisionCheck(3, 28); // �÷��̾�, ���
	CCollisionMgr::GetInst()->CollisionCheck(4, 28); // ����, ���
	CCollisionMgr::GetInst()->CollisionCheck(6, 28); // ��ȣ�ۿ�, ���
	CCollisionMgr::GetInst()->CollisionCheck(8, 28); // ����, ���
	CCollisionMgr::GetInst()->CollisionCheck(6, 8); // ����, ��ȣ�ۿ�

	CCollisionMgr::GetInst()->CollisionCheck(3, 10); // �÷��̾�, ���� ����
	CCollisionMgr::GetInst()->CollisionCheck(6, 10); // ��ȣ�ۿ�, ���� ����
	CCollisionMgr::GetInst()->CollisionCheck(10, 10); // ���� ����, ���� ����
	CCollisionMgr::GetInst()->CollisionCheck(28, 10); // ���, ���� ����
	CCollisionMgr::GetInst()->CollisionCheck(29, 10); // ī�޶� ���, ���� ����

	CCollisionMgr::GetInst()->CollisionCheck(3, 29); // �÷��̾�, ī�޶� ���
	CCollisionMgr::GetInst()->CollisionCheck(4, 29); // ����, ī�޶� ���
	CCollisionMgr::GetInst()->CollisionCheck(6, 29); // ��ȣ�ۿ�, ī�޶� ���
	CCollisionMgr::GetInst()->CollisionCheck(29, 30); // ī�޶� ���, ī�޶�
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

void CTestLevel::PlayerAni(CGameObject* pPlayer)
{
	Ptr<CFlipBook> pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle.flip");
	pPlayer->FlipBookComponent()->AddFlipBook(0, pFlipBook);
	pPlayer->FlipBookComponent()->Play((int)OGU_FLIPBOOK_IDX::OGU_IDLE, 7, true);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle_back.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_IDLE_BACK, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_IDLE_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_IDLE_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_IDLE_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_IDLE_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_IDLE_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_IDLE_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_walk_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_WALK_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_walk_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_WALK_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_walk_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_WALK_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_walk_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_WALK_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_walk_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_WALK_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_walk_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_WALK_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_walk_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_WALK_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_walk_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_WALK_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_run_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_RUN_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_run_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_RUN_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_run_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_RUN_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_run_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_RUN_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_run_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_RUN_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_run_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_RUN_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_run_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_RUN_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_run_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_RUN_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_idle_dance.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_IDLE_DANCE, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_dance.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_DANCE, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_roll_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_ROLL_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_roll_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_ROLL_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_roll_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_ROLL_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_roll_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_ROLL_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_roll_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_ROLL_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_roll_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_ROLL_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_roll_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_ROLL_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_roll_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_ROLL_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_swing_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_SWING_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_swing_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_SWING_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_swing_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_SWING_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_swing_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_SWING_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_swing_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_SWING_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_swing_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_SWING_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_swing_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_SWING_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Ogu\\ogu_swing_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_SWING_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle1_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle1_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle1_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle1_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle1_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle1_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle1_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle1_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE1_RIGHTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle2_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle2_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle2_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle2_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle2_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle2_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle2_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_idle2_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_IDLE2_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_walk_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_walk_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_walk_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_walk_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_RIGHT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_walk_leftdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_LEFTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_walk_leftup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_LEFTUP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_walk_rightdown.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_RIGHTDOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguLift\\ogu_lift_walk_rightup.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_LIFT_WALK_RIGHTUP, pFlipBook);

	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguPush\\ogu_push_down.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_PUSH_DOWN, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguPush\\ogu_push_up.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_PUSH_UP, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguPush\\ogu_push_left.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_PUSH_LEFT, pFlipBook);
	pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\OguPush\\ogu_push_right.flip");
	pPlayer->FlipBookComponent()->AddFlipBook((int)OGU_FLIPBOOK_IDX::OGU_PUSH_RIGHT, pFlipBook);
}
