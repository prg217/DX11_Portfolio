#include "pch.h"
#include "CCactusNeedleScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CSpitCactusScript.h"

#include "CHPScript.h"

CCactusNeedleScript::CCactusNeedleScript()
	: CScript(UINT(SCRIPT_TYPE::CACTUSNEEDLESCRIPT))
	, m_Type(SpitCactusType::Nomal)
	, m_Dir(NeedleDir::UP)
	, m_Speed(200.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Type", &m_Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Dir", &m_Dir);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_Speed);
}

CCactusNeedleScript::CCactusNeedleScript(const CCactusNeedleScript& _Origin)
	: CScript(_Origin)
	, m_Type(_Origin.m_Type)
	, m_Dir(_Origin.m_Dir)
	, m_Speed(200.f)
{
}

CCactusNeedleScript::~CCactusNeedleScript()
{
}

void CCactusNeedleScript::Begin()
{
	// 레이어 바꾸기
	int LayerIdx = GetOwner()->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);

	pLayer->LayerChange(GetOwner(), 10);
}

void CCactusNeedleScript::Tick()
{
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();

	switch (m_Dir)
	{
	case NeedleDir::UP:
		pos.y += DT * m_Speed;
		break;
	case NeedleDir::DOWN:
		pos.y -= DT * m_Speed;
		break;
	case NeedleDir::LEFT:
		pos.x -= DT * m_Speed;
		break;
	case NeedleDir::RIGHT:
		pos.x += DT * m_Speed;
		break;
	default:
		break;
	}

	GetOwner()->Transform()->SetRelativePos(pos);
}

void CCactusNeedleScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 플레이어에게 Hit보내기
	if (_OtherObject->GetLayerIdx() == 3)
	{
		CHPScript* hpScript = dynamic_cast<CHPScript*>(_OtherObject->GetScript("CHPScript"));

		if (hpScript != nullptr)
		{
			hpScript->Hit();
			DeleteObject(GetOwner());
		}
	}
}

void CCactusNeedleScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCactusNeedleScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCactusNeedleScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(SpitCactusType), 1, _File);
	fwrite(&m_Speed, sizeof(float), 1, _File); 
	fwrite(&m_Dir, sizeof(NeedleDir), 1, _File);
}

void CCactusNeedleScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(SpitCactusType), 1, _File);
	fread(&m_Speed, sizeof(float), 1, _File);
	fread(&m_Dir, sizeof(NeedleDir), 1, _File);
}
