#include "pch.h"
#include "CBossInScript.h"

#include "CBugBossScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CBossInScript::CBossInScript()
	: CScript(UINT(SCRIPT_TYPE::BOSSINSCRIPT))
	, m_BossType(BossType::BugBoss)
	, m_Door1(nullptr)
	, m_Door2(nullptr)
	, m_Door3(nullptr)
	, m_SaveDoorPosY(0.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "BossType", &m_BossType);
}

CBossInScript::CBossInScript(const CBossInScript& _Origin)
	: CScript(_Origin)
	, m_BossType(BossType::BugBoss)
	, m_Door1(nullptr)
	, m_Door2(nullptr)
	, m_Door3(nullptr)
	, m_SaveDoorPosY(0.f)
{
}

CBossInScript::~CBossInScript()
{
}

void CBossInScript::Begin()
{
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_Door1 = curLevel->FindObjectByName(L"Door1");
	m_Door2 = curLevel->FindObjectByName(L"Door2");
	m_Door3 = curLevel->FindObjectByName(L"Door3");

	m_SaveDoorPosY = m_Door1->Transform()->GetRelativePos().y;
}

void CBossInScript::Tick()
{
}

void CBossInScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 3)
	{
		switch (m_BossType)
		{
		case BossType::BugBoss:
		{
			CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			CGameObject* bossObj = curLevel->FindObjectByName(L"BugBoss");
			CBugBossScript* bossScript = dynamic_cast<CBugBossScript*>(bossObj->GetScript("CBugBossScript"));
			bossScript->Active();

			// 뒤에 문을 다시 세운다.
			m_Door1->SpriteComponent()->SliceAmend(false);
			m_Door2->SpriteComponent()->SliceAmend(false);
			m_Door3->SpriteComponent()->SliceAmend(false);

			Vec3 pos = m_Door1->Transform()->GetRelativePos();
			m_Door1->Transform()->SetRelativePos(Vec3(pos.x, m_SaveDoorPosY, pos.z));
			pos = m_Door2->Transform()->GetRelativePos();
			m_Door2->Transform()->SetRelativePos(Vec3(pos.x, m_SaveDoorPosY, pos.z));
			pos = m_Door3->Transform()->GetRelativePos();
			m_Door3->Transform()->SetRelativePos(Vec3(pos.x, m_SaveDoorPosY, pos.z));

			m_Door2->AddComponent(new CCollider2D);
			m_Door2->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
			m_Door2->Collider2D()->SetScale(Vec3(3.f, 0.3f, 1.f));
		}
			break;
		default:
			break;
		}

		DeleteObject(GetOwner());
	}
}

void CBossInScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossInScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossInScript::SaveToFile(FILE* _File)
{
	fwrite(&m_BossType, sizeof(BossType), 1, _File);
}

void CBossInScript::LoadFromFile(FILE* _File)
{
	fread(&m_BossType, sizeof(BossType), 1, _File);
}
