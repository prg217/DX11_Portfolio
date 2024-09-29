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
	, m_InteractionObj1(nullptr)
	, m_InteractionObj2(nullptr)
	, m_LinkObj(nullptr)
	, m_LinkOK(false)
	, m_PuzzleObjNum(0)
	, m_Open(false)
	, m_GoalPosY(0)
	, m_Speed(10.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_JellyType);
	AddScriptParam(SCRIPT_PARAM::INT, "PuzzleType", &m_PuzzleType);
	AddScriptParam(SCRIPT_PARAM::INT, "PuzzleObjNumber", &m_PuzzleObjNum);
	AddScriptParam(SCRIPT_PARAM::GAMEOBJECT, "InteractionObj", &m_InteractionObj1);
}

CJellyPushFrameScript::CJellyPushFrameScript(const CJellyPushFrameScript& _Origin)
	: CScript(_Origin)
	, m_JellyType(JellyPushType::NONE)
	, m_InteractionObj1(nullptr)
	, m_InteractionObj2(nullptr)
	, m_LinkObj(nullptr)
	, m_LinkOK(false)
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
			m_InteractionObj1 = curLevel->FindObjectByName(L"StoneBlock");
			break;
		case 1:
			m_InteractionObj1 = curLevel->FindObjectByName(L"SB1");
			break;
		case 2:
			if (wcscmp(GetOwner()->GetName().c_str(), L"Red2F_0") == 0)
			{
				m_LinkObj = curLevel->FindObjectByName(L"Red2F_1");
			}
			else if (wcscmp(GetOwner()->GetName().c_str(), L"Red2F_1") == 0)
			{
				m_LinkObj = curLevel->FindObjectByName(L"Red2F_0");
			}
			m_InteractionObj1 = curLevel->FindObjectByName(L"SB2");
			m_InteractionObj2 = curLevel->FindObjectByName(L"SB3");
			break;
		default:
			break;
		}
		break;
	case PuzzleType::FLOWER:
		switch (m_PuzzleObjNum)
		{
		case 0:
			m_InteractionObj1 = curLevel->FindObjectByName(L"FlowerLight");
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

}

void CJellyPushFrameScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetScript("CJellyPushScript") != nullptr)
	{
		CScript* script = _OtherObject->GetScript("CJellyPushScript");
		CJellyPushScript* jellyScript = dynamic_cast<CJellyPushScript*>(script);

		if (jellyScript->GetJellyPushType() == m_JellyType && !m_Open)
		{
			if (m_LinkObj != nullptr)
			{
				CJellyPushFrameScript* script = dynamic_cast<CJellyPushFrameScript*>(m_LinkObj->GetScript("CJellyPushFrameScript"));
				if (script->GetLinkOK() == false)
				{
					m_LinkOK = true;

					// 더 이상 반응 할 수 없게 콜라이더를 제거해준다.
					if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
					{
						DeleteComponent(GetOwner(), COMPONENT_TYPE::COLLIDER2D);
					}
					// 더 이상 충돌 할 수 없게 콜라이더를 제거해준다.
					if (m_InteractionObj1->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
					{
						DeleteComponent(m_InteractionObj1, COMPONENT_TYPE::COLLIDER2D);
					}

					return;
				}
			}

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
				if (m_InteractionObj1->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
				{
					DeleteComponent(m_InteractionObj1, COMPONENT_TYPE::COLLIDER2D);
				}
				break;
			default:
				break;
			}
		}

		// 색상에 관계 없이 꽃이 핀다.
		if (m_PuzzleType == PuzzleType::FLOWER)
		{
			FlowerBloom(jellyScript->GetJellyPushType());
		}
	}
}

void CJellyPushFrameScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetScript("CJellyPushScript") != nullptr)
	{
		if (m_PuzzleType == PuzzleType::FLOWER)
		{
			FlowerFall();
		}
	}

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
	m_InteractionObj1->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	if (m_InteractionObj2 != nullptr)
	{
		m_InteractionObj2->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	}

	if (m_InteractionObj1->GetChildren().size() != 0)
	{
		CGameObject* bug = m_InteractionObj1->GetChildren()[0];
		bug->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	}
	if (m_InteractionObj2 != nullptr)
	{
		if (m_InteractionObj2->GetChildren().size() != 0)
		{
			CGameObject* bug = m_InteractionObj2->GetChildren()[0];
			bug->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
		}
	}

	if (m_Pos.y <= m_GoalPosY)
	{
		if (m_InteractionObj1->GetChildren().size() != 0)
		{
			for (auto i : m_InteractionObj1->GetChildren())
			{
				DeleteObject(i);
			}
		}
		if (m_InteractionObj2 != nullptr)
		{
			if (m_InteractionObj2->GetChildren().size() != 0)
			{
				for (auto i : m_InteractionObj2->GetChildren())
				{
					DeleteObject(i);
				}
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

	m_InteractionObj1->Transform()->SetRelativePos(m_Pos.x, m_Pos.y, 489.5f);
	if (m_InteractionObj2 != nullptr)
	{
		m_InteractionObj2->Transform()->SetRelativePos(m_InteractionObj2->Transform()->GetRelativePos().x, m_Pos.y, 489.5f);
	}
}

void CJellyPushFrameScript::StoneBlock()
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_168_GateOnceOff.wav");
	pSound->Play(1, 1.f, false);

	// m_StoneBlock을 연다.
	m_Pos = m_InteractionObj1->Transform()->GetRelativePos();
	m_GoalPosY = m_Pos.y - 15.f;

	Ptr<CSprite> sprite = m_InteractionObj1->SpriteComponent()->GetSprite();
	m_Slice = sprite->GetSliceUV() * sprite->GetTexSize();

	// 카메라 포커스
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
	CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
	CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
	cameraScript->Focus(m_InteractionObj1);

	// 그림 빛 추가
	if (m_InteractionObj1->GetChildren().size() != 0 && !m_Open)
	{
		CGameObject* bug = m_InteractionObj1->GetChildren()[0];
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
		AddChildObject(m_InteractionObj1, pLight);
	}
	if (m_InteractionObj2 != nullptr)
	{
		if (m_InteractionObj2->GetChildren().size() != 0 && !m_Open)
		{
			CGameObject* bug = m_InteractionObj2->GetChildren()[0];
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
			AddChildObject(m_InteractionObj2, pLight);
		}
	}

	m_Open = true;
}

void CJellyPushFrameScript::FlowerBloom(JellyPushType _JellyPushType)
{
	CScript* script = m_InteractionObj1->GetScript("CFlowerLightScript");
	CFlowerLightScript* flowerLightScript = dynamic_cast<CFlowerLightScript*>(script);
	
	flowerLightScript->Bloom(_JellyPushType);
}

void CJellyPushFrameScript::FlowerFall()
{
	CScript* script = m_InteractionObj1->GetScript("CFlowerLightScript");
	CFlowerLightScript* flowerLightScript = dynamic_cast<CFlowerLightScript*>(script);

	flowerLightScript->Fall();
}
