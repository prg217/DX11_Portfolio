#include "pch.h"
#include "CDoorOpenScript.h"

#include "CCameraPlayerTrackingScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CDoorOpenScript::CDoorOpenScript()
	: CScript(UINT(SCRIPT_TYPE::DOOROPENSCRIPT))
	, m_Door1(nullptr)
	, m_Door2(nullptr)
	, m_Door3(nullptr)
	, m_Open(false)
	, m_GoalPosY(0)
	, m_Speed(10.f)
{
}

CDoorOpenScript::CDoorOpenScript(const CDoorOpenScript& _Origin)
	: CScript(_Origin)
	, m_Door1(nullptr)
	, m_Door2(nullptr)
	, m_Door3(nullptr)
	, m_Open(false)
	, m_GoalPosY(0)
	, m_Speed(10.f)
{
}

CDoorOpenScript::~CDoorOpenScript()
{
}

void CDoorOpenScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_Door1 = curLevel->FindObjectByName(L"Door1");
	m_Door2 = curLevel->FindObjectByName(L"Door2");
	m_Door3 = curLevel->FindObjectByName(L"Door3");
}

void CDoorOpenScript::Tick()
{
	if (m_Open)
	{
		Open();
	}
}

void CDoorOpenScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 문을 연다.
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_168_GateOnceOff.wav");
	pSound->Play(1, 1.f, false);

	m_Pos = m_Door1->Transform()->GetRelativePos();
	m_GoalPosY = m_Pos.y - 10.f;
	Ptr<CSprite> sprite = m_Door1->SpriteComponent()->GetSprite();
	m_Slice = sprite->GetSliceUV() * sprite->GetTexSize();

	// 그림 빛 추가
	if (m_Door1->GetChildren().size() != 0 && !m_Open)
	{
		CGameObject* bug = m_Door1->GetChildren()[0];
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
		AddChildObject(m_Door1, pLight);
	}
	if (m_Door2->GetChildren().size() != 0 && !m_Open)
	{
		CGameObject* bug = m_Door2->GetChildren()[0];
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
		AddChildObject(m_Door2, pLight);
	}
	if (m_Door3->GetChildren().size() != 0 && !m_Open)
	{
		CGameObject* bug = m_Door3->GetChildren()[0];
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
		AddChildObject(m_Door3, pLight);
	}

	m_Open = true;
}

void CDoorOpenScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CDoorOpenScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CDoorOpenScript::SaveToFile(FILE* _File)
{
}

void CDoorOpenScript::LoadFromFile(FILE* _File)
{
}

void CDoorOpenScript::Open()
{
	// 카메라 포커스
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
	CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
	CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
	cameraScript->Focus(m_Door2);

	m_Pos.y -= m_Speed * DT;

	// 스프라이트 Slice 부분을 줄여 삐져나오는 아래 부분을 없애준다.
	m_Slice.y -= 60.f * DT;
	m_Door1->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	m_Door2->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	m_Door3->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	if (m_Door1->GetChildren().size() != 0)
	{
		CGameObject* bug = m_Door1->GetChildren()[0];
		bug->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	}
	if (m_Door2->GetChildren().size() != 0)
	{
		CGameObject* bug = m_Door2->GetChildren()[0];
		bug->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	}
	if (m_Door3->GetChildren().size() != 0)
	{
		CGameObject* bug = m_Door3->GetChildren()[0];
		bug->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	}

	if (m_Pos.y <= m_GoalPosY)
	{
		if (m_Door1->GetChildren().size() != 0)
		{
			for (auto i : m_Door1->GetChildren())
			{
				DeleteObject(i);
			}
		}
		if (m_Door2->GetChildren().size() != 0)
		{
			for (auto i : m_Door2->GetChildren())
			{
				DeleteObject(i);
			}
		}
		if (m_Door3->GetChildren().size() != 0)
		{
			for (auto i : m_Door3->GetChildren())
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

		DeleteObject(GetOwner());
		m_Door1->DeregisterComponent(COMPONENT_TYPE::COLLIDER2D);
		m_Door2->DeregisterComponent(COMPONENT_TYPE::COLLIDER2D);
		m_Door3->DeregisterComponent(COMPONENT_TYPE::COLLIDER2D);
	}

	m_Door1->Transform()->SetRelativePos(m_Pos.x, m_Pos.y, 2890.f);
	m_Door2->Transform()->SetRelativePos(m_Door2->Transform()->GetRelativePos().x, m_Pos.y, 2890.f);
	m_Door3->Transform()->SetRelativePos(m_Door3->Transform()->GetRelativePos().x, m_Pos.y, 2890.f);
}
