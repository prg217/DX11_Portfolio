#include "pch.h"
#include "MenuUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Scripts/CScriptMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CScript.h>

#include "CEditorMgr.h"
#include "Inspector.h"
#include "CLevelSaveLoad.h"


MenuUI::MenuUI()
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Tick()
{
    if (!IsActive())
        return;

    if (ImGui::BeginMainMenuBar())
    {

        Update();

        ImGui::EndMainMenuBar();
    }
}


void MenuUI::Update()
{
	File();

	Level();

	GameObject();

	Assets();	
}

void MenuUI::File()
{
	if (ImGui::BeginMenu("File"))
	{
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

		if (ImGui::MenuItem("Level Save"))
		{
			CLevelSaveLoad::SaveLevelExplorer(pCurLevel);
		}

		if (ImGui::MenuItem("Level Load"))
		{
			ChangeLevel(CLevelSaveLoad::LoadLevelExplorer(), LEVEL_STATE::STOP);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Level()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	ImGui::BeginDisabled(!pCurLevel);

	if (ImGui::BeginMenu("Level"))
	{
		LEVEL_STATE State = LEVEL_STATE::STOP;
		if (nullptr != pCurLevel)
			State = pCurLevel->GetState();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY == State);
		if (ImGui::MenuItem("Play"))
		{			
			if (LEVEL_STATE::STOP == State)
			{
				wstring strLevelPath = CPathMgr::GetInst()->GetContentPath();
				strLevelPath += L"level\\Temp.lv";
				CLevelSaveLoad::SaveLevel(strLevelPath, pCurLevel);
			}
			
			ChangeLevelState(LEVEL_STATE::PLAY);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY != State);
		if (ImGui::MenuItem("Pause"))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::STOP == State);
		if (ImGui::MenuItem("Stop"))
		{
			wstring StrLevelLoadPath = CPathMgr::GetInst()->GetContentPath();
			StrLevelLoadPath += L"level\\Temp.lv";
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(StrLevelLoadPath);
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

			// Inspector Clear 하기 (이전 오브젝트 정보를 보여주고 있을 수가 있기 때문에)				
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);
		}
		ImGui::EndDisabled();

		ImGui::EndMenu();
	}

	ImGui::EndDisabled();
}

void MenuUI::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object"))
		{
			CGameObject* pObj = new CGameObject;
			pObj->SetName(L"new Obj");
			pObj->AddComponent(new CTransform);
			pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
			pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 0.f));

			CreateObject(pObj, 0);
		}

		if (ImGui::BeginMenu("Add Component"))
		{
			if (ImGui::MenuItem("MeshRender"))
			{
				// 인스펙터
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

				// 타겟 오브젝트 알아냄
				CGameObject* pObject = pInspector->GetTargetObject();
				if (nullptr != pObject)
				{
					pObject->AddComponent(new CMeshRender);
				}
			}
			if (ImGui::MenuItem("Collider2D"))
			{
				// 인스펙터
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

				// 타겟 오브젝트 알아냄
				CGameObject* pObject = pInspector->GetTargetObject();
				if (nullptr != pObject)
				{
					pObject->AddComponent(new CCollider2D);
					pObject->Collider2D()->SetScale(Vec3(1.f, 1.f, 0.f));
				}
			}
			ImGui::MenuItem("Camera");

			ImGui::EndMenu();
		}

		AddScript();


		ImGui::EndMenu();
	}
}

void MenuUI::AddScript()
{
	if (ImGui::BeginMenu("Add Script"))
	{
		vector<wstring> vecScriptsName;
		CScriptMgr::GetScriptInfo(vecScriptsName);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{			
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				//CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);

				// 인스펙터
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

				// 타겟 오브젝트 알아냄
				CGameObject* pObject = pInspector->GetTargetObject();
				if (nullptr != pObject)
				{
					// 오브젝트에, 선택한 스크립트를 추가해줌
					CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
					pObject->AddComponent(pScript);
				}
			}
		}

		ImGui::EndMenu();
	}

}


void MenuUI::Assets()
{
	if (ImGui::BeginMenu("Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<CMaterial> pMtrl = new CMaterial;						
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			CAssetMgr::GetInst()->AddAsset<CMaterial>(Key, pMtrl);
			pMtrl->Save(Key);
		}



		EditorUI* pSpriteEditor = CEditorMgr::GetInst()->FindEditorUI("SpriteEditor");				
		bool IsSpriteActive = pSpriteEditor->IsActive();

		if (ImGui::MenuItem("Sprite Editor", nullptr, &IsSpriteActive))
		{			
			CEditorMgr::GetInst()->FindEditorUI("SpriteEditor")->SetActive(IsSpriteActive);
		}
		
		EditorUI* pAnimationEditor = CEditorMgr::GetInst()->FindEditorUI("AnimationEditor");
		bool IsAnimationActive = pAnimationEditor->IsActive();

		if (ImGui::MenuItem("Animation Editor", nullptr, &IsAnimationActive))
		{
			CEditorMgr::GetInst()->FindEditorUI("AnimationEditor")->SetActive(IsAnimationActive);
		}

		EditorUI* pTileMapEditor = CEditorMgr::GetInst()->FindEditorUI("TileMapEditor");
		bool IsTileMapActive = pTileMapEditor->IsActive();

		if (ImGui::MenuItem("TileMap Editor", nullptr, &IsTileMapActive))
		{
			CEditorMgr::GetInst()->FindEditorUI("TileMapEditor")->SetActive(IsTileMapActive);
		}
		
		ImGui::EndMenu();
	}
}



wstring MenuUI::GetAssetKey(ASSET_TYPE _Type, const wstring& _KeyFormat)
{
	wstring Key;

	switch (_Type)
	{	
	case ASSET_TYPE::MATERIAL:
	{
		Key = wstring(L"material\\") + _KeyFormat + L" %d.mtrl";
	}	
		break;
	case ASSET_TYPE::PREFAB:
	{
		Key = wstring(L"prefab\\") + _KeyFormat + L" %d.pref";
	}
		break;
	case ASSET_TYPE::SPRITE:
	{
		Key = wstring(L"sprite\\") + _KeyFormat + L" %d.sprite";
	}
		break;
	case ASSET_TYPE::FLIPBOOK:
	{
		Key = wstring(L"flipbook\\") + _KeyFormat + L" %d.flip";
	}
		break;	
	default:
		assert(nullptr);
		break;
	}

	wchar_t szKey[255] = {};
	wstring FilePath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < 0xffffffff; ++i)
	{		
		swprintf_s(szKey, 255, Key.c_str(), i);
		
		if (false == std::filesystem::exists(FilePath + szKey))
		{
			break;
		}
	}

	return szKey;
}
