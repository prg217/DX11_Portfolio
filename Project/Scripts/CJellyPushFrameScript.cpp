#include "pch.h"
#include "CJellyPushFrameScript.h"

#include "CJellyPushScript.h"
#include "CCameraPlayerTrackingScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CJellyPushFrameScript::CJellyPushFrameScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHFRAMESCRIPT))
	, m_Type(JellyPushType::NONE)
	, m_StoneBlock(nullptr)
	, m_PuzzleNum(0)
	, m_Open(false)
	, m_GoalPosY(0)
	, m_Speed(10.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_Type);
	AddScriptParam(SCRIPT_PARAM::INT, "PuzzleNumber", &m_PuzzleNum);
	AddScriptParam(SCRIPT_PARAM::GAMEOBJECT, "StoneBlock", &m_StoneBlock);
}

CJellyPushFrameScript::CJellyPushFrameScript(const CJellyPushFrameScript& _Origin)
	: CScript(_Origin)
	, m_Type(JellyPushType::NONE)
	, m_StoneBlock(nullptr)
	, m_PuzzleNum(0)
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
	switch (m_PuzzleNum)
	{
	case 0:
		m_StoneBlock = curLevel->FindObjectByName(L"StoneBlock");
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
	// ���� Ÿ�԰� ���� ������ ���� ����(������ 25��ŭ �̵�)
	// ū ������ �� �� ���� ���߸� ������ �� ���� �ƿ� �����ϰ� ��縸 ����� ���� �����?
	if (_OtherObject->GetScript("CJellyPushScript") != nullptr)
	{
		CScript* script = _OtherObject->GetScript("CJellyPushScript");
		CJellyPushScript* jellyScript = dynamic_cast<CJellyPushScript*>(script);

		if (jellyScript->GetJellyPushType() == m_Type)
		{
			// m_StoneBlock�� ����.
			m_Pos = m_StoneBlock->Transform()->GetRelativePos();
			m_GoalPosY = m_Pos.y - 15.f;

			Ptr<CSprite> sprite = m_StoneBlock->SpriteComponent()->GetSprite();
			m_Slice = sprite->GetSliceUV() * sprite->GetTexSize();
			
			// ī�޶� ��Ŀ��
			CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
			CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
			CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
			cameraScript->Focus(m_StoneBlock);

			// �׸� �� �߰�
			if (m_StoneBlock->GetChildren().size() != 0 && !m_Open)
			{
				CGameObject* bug = m_StoneBlock->GetChildren()[0];
				bug->SpriteComponent()->AddColor(true, Vec3(0.7f, 0.99f, 0.8f));

				// ����Ʈ ����Ʈ �߰�
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
				AddChildObject(m_StoneBlock, pLight);
			}

			// �� �̻� ���� �� �� ���� �ݶ��̴��� �������ش�.
			if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
			{
				DeleteComponent(GetOwner(), COMPONENT_TYPE::COLLIDER2D);
			}
			// �� �̻� �浹 �� �� ���� �ݶ��̴��� �������ش�.
			if (m_StoneBlock->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
			{
				DeleteComponent(m_StoneBlock, COMPONENT_TYPE::COLLIDER2D);
			}

			m_Open = true;
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
	fwrite(&m_Type, sizeof(JellyPushType), 1, _File);
	fwrite(&m_PuzzleNum, sizeof(int), 1, _File);
	//fwrite(&m_StoneBlock, sizeof(CGameObject*), 1, _File);
}

void CJellyPushFrameScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(JellyPushType), 1, _File);
	fread(&m_PuzzleNum, sizeof(int), 1, _File);
	//fread(&m_StoneBlock, sizeof(CGameObject*), 1, _File);
}

void CJellyPushFrameScript::Open()
{
	// ��������Ʈ ������ �Ʒ�...�ű⵵ ���� ���̱�?
	m_Pos.y -= m_Speed * DT;

	m_Slice.y -= 40.f * DT;
	m_StoneBlock->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	if (m_StoneBlock->GetChildren().size() != 0)
	{
		CGameObject* bug = m_StoneBlock->GetChildren()[0];
		bug->SpriteComponent()->SliceAmend(true, Vec2(m_Slice.x, m_Slice.y));
	}

	if (m_Pos.y <= m_GoalPosY)
	{
		if (m_StoneBlock->GetChildren().size() != 0)
		{
			for (auto i : m_StoneBlock->GetChildren())
			{
				DeleteObject(i);
			}
		}

		// ī�޶� ��Ŀ��
		CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
		CGameObject* mainCamera = curLevel->FindObjectByName(L"MainCamera");
		CScript* script = mainCamera->GetScript("CCameraPlayerTrackingScript");
		CCameraPlayerTrackingScript* cameraScript = dynamic_cast<CCameraPlayerTrackingScript*>(script);
		cameraScript->Focus(curLevel->FindObjectByName(L"Player"));

		m_Open = false;
	}

	m_StoneBlock->Transform()->SetRelativePos(m_Pos.x, m_Pos.y, 490.f);
}
