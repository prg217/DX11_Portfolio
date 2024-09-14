#include "pch.h"
#include "CJellyPushFrameScript.h"

#include "CJellyPushScript.h"

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
			m_Open = true;
			m_Pos = m_StoneBlock->Transform()->GetRelativePos();
			m_GoalPosY = m_Pos.y - 15.f;
			if (GetOwner()->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
			{
				DeleteComponent(GetOwner(), COMPONENT_TYPE::COLLIDER2D);
			}
			if (m_StoneBlock->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
			{
				DeleteComponent(m_StoneBlock, COMPONENT_TYPE::COLLIDER2D);
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

	Ptr<CSprite> sprite = m_StoneBlock->SpriteComponent()->GetSprite();
	Vec2 slice = sprite->GetSliceUV() * sprite->GetTexSize();
	slice.y -= 40.f * DT;
	sprite->SetSlice(Vec2(slice.x, slice.y));

	if (m_Pos.y <= m_GoalPosY)
	{
		m_Open = false;
	}

	m_StoneBlock->Transform()->SetRelativePos(m_Pos.x, m_Pos.y, 490.f);
}
