#include "pch.h"
#include "CJellyPushFrameScript.h"

#include "CJellyPushScript.h"
#include "CCameraPlayerTrackingScript.h"
#include "CFlowerLightScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CJellyPushFrameScript::CJellyPushFrameScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHFRAMESCRIPT))
	, m_JellyType(JellyPushType::NONE)
	, m_InteractionObj(nullptr)
	, m_PuzzleObjNum(0)
	, m_Open(false)
	, m_GoalPosY(0)
	, m_Speed(10.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_JellyType);
	AddScriptParam(SCRIPT_PARAM::INT, "PuzzleType", &m_PuzzleType);
	AddScriptParam(SCRIPT_PARAM::INT, "PuzzleObjNumber", &m_PuzzleObjNum);
	AddScriptParam(SCRIPT_PARAM::GAMEOBJECT, "InteractionObj", &m_InteractionObj);
}

CJellyPushFrameScript::CJellyPushFrameScript(const CJellyPushFrameScript& _Origin)
	: CScript(_Origin)
	, m_JellyType(JellyPushType::NONE)
	, m_InteractionObj(nullptr)
	, m_PuzzleObjNum(0)
	, m_Open(false)
	, m_GoalPosY(0)
	, m_Speed(10.f)
{
}

CJellyPushFrameScript::~CJellyPushFrameScript()
{
}

void CJellyPushFrameScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	switch (m_PuzzleType)
	{
	case PuzzleType::STONE_BLOCK:
		switch (m_PuzzleObjNum)
		{
		case 0:
			m_InteractionObj = curLevel->FindObjectByName(L"StoneBlock");
			break;
		default:
			break;
		}
		break;
	case PuzzleType::FLOWER:
		switch (m_PuzzleObjNum)
		{
		case 0:
			m_InteractionObj = curLevel->FindObjectByName(L"FlowerLight");
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}


}

void CJellyPushFrameScript::Tick()
{
	if (m_Open)
	{
		Open();
	}
}

void CJellyPushFrameScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetScript("CJellyPushScript") != nullptr)
	{
		CScript* script = _OtherObject->GetScript("CJellyPushScript");
		CJellyPushScript* jellyScript = dynamic_cast<CJellyPushScript*>(script);

		if (jellyScript->GetJellyPushType() == m_JellyType)
		{
			switch (m_PuzzleType)
			{
			case PuzzleType::STONE_BLOCK:
				StoneBlock();

				// 더 이상 반응 할 수 없게 콜라이더를 제거해준다.
				if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
				{
					DeleteComponent(GetOwner(), COMPONENT_TYPE::COLLIDER2D);
				}
				// 더 이상 충돌 할 수 없게 콜라이더를 제거해준다.
				if (m_InteractionObj->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
				{
					DeleteComponent(m_InteractionObj, COMPONENT_TYPE::COLLIDER2D);
				}
				break;
			case PuzzleType::FLOWER:
				Flower(_OtherObject);
				break;
			default:
				break;
			}
		}
	}
}

void CJellyPushFrameScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushFrameScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushFrameScript::SaveToFile(FILE* _File)
{
	fwrite(&m_JellyType, sizeof(JellyPushType), 1, _File);
	fwrite(&m_PuzzleObjNum, sizeof(int), 1, _File);
	fwrite(&m_PuzzleType, sizeof(PuzzleType), 1, _File);
}

void CJellyPushFrameScript::LoadFromFile(FILE* _File)
{
	fread(&m_JellyType, sizeof(JellyPushType), 1, _File);
	fread(&m_PuzzleObjNum, sizeof(int), 1, _File);
	fread(&m_PuzzleType, sizeof(PuzzleType), 1, _File);
}

void CJellyPushFrameScript::Open()
{
	m_Pos.y -= m_Speed * DT;

	// 스프라이트 Slice 부분을 줄여 삐져나오는 아래 부분을 없애준다.
	m_Slice.y -= 40.f * DT;
	m_InteractionObj->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	if (m_InteractionObj->GetChildren().size() != 0)
	{
		CGameObject* bug = m_InteractionObj->GetChildren()[0];
		bug->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	}

	if (m_Pos.y <= m_GoalPosY)
	{
		if (m_InteractionObj->GetChildren().size() != 0)
		{
			for (auto i : m_InteractionObj->GetChildren())
			{
				DeleteObject(i);
			}
		}

		// 카메라 포커스
		CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
		CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
		CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
		cameraScript->Focus(curLevel->FindObjectByName(L"Player"));

		m_Open = false;
	}

	m_InteractionObj->Transform()->SetRelativePos(m_Pos.x, m_Pos.y, 490.f);
}

void CJellyPushFrameScript::StoneBlock()
{
	// m_StoneBlock을 연다.
	m_Pos = m_InteractionObj->Transform()->GetRelativePos();
	m_GoalPosY = m_Pos.y - 15.f;

	Ptr<CSprite> sprite = m_InteractionObj->SpriteComponent()->GetSprite();
	m_Slice = sprite->GetSliceUV() * sprite->GetTexSize();

	// 카메라 포커스
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
	CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
	CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
	cameraScript->Focus(m_InteractionObj);

	// 그림 빛 추가
	if (m_InteractionObj->GetChildren().size() != 0 && !m_Open)
	{
		CGameObject* bug = m_InteractionObj->GetChildren()[0];
		bug->SpriteComponent()->AddColor(true, Vec3(0.7f, 0.99f, 0.8f));

		// 포인트 라이트 추가
		CGameObject* pLight = new CGameObject;
		pLight->SetName(L"StoneBlock_Light");
		pLight->AddComponent(new CTransform);
		pLight->AddComponent(new CLight2D);

		pLight->Transform()->SetRelativePos(Vec3(0, 0, 0));
		pLight->Transform()->SetRelativeScale(Vec3(1, 1, 0));

		pLight->Light2D()->SetLightColor(Vec3(0.62f, 0.99f, 0.97f));
		pLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
		pLight->Light2D()->SetRadius(35.f);

		CreateObject(pLight, 0);
		AddChildObject(m_InteractionObj, pLight);
	}

	m_Open = true;
}

void CJellyPushFrameScript::Flower(CGameObject* _Other)
{
	CScript* script = _Other->GetScript("CFlowerLightScript");
	CFlowerLightScript* flowerLightScript = dynamic_cast<CFlowerLightScript*>(script);
	
	flowerLightScript->Bloom();
}
