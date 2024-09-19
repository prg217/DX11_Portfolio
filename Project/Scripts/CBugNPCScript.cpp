#include "pch.h"
#include "CBugNPCScript.h"

#include "CJellyPushScript.h"
#include "CFlowerLightAreaScript.h"
#include "CNPCScript.h"

CBugNPCScript::CBugNPCScript()
	: CScript(UINT(SCRIPT_TYPE::BUGNPCSCRIPT))
	, m_JellyPushType(JellyPushType::NONE)
	, m_SameType(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_JellyPushType);
}

CBugNPCScript::CBugNPCScript(const CBugNPCScript& _Origin)
	: CScript(_Origin)
	, m_JellyPushType(_Origin.m_JellyPushType)
	, m_SameType(false)
{
}

CBugNPCScript::~CBugNPCScript()
{
}

void CBugNPCScript::Begin()
{
}

void CBugNPCScript::Tick()
{
}

void CBugNPCScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugNPCScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	CFlowerLightAreaScript* areaScript = dynamic_cast<CFlowerLightAreaScript*>(_OtherObject->GetScript("CFlowerLightAreaScript"));
	if (areaScript != nullptr)
	{
		CNPCScript* npcScript = dynamic_cast<CNPCScript*>(GetOwner()->GetScript("CNPCScript"));
		// 꽃의 빛 색이 내 타입과 같을 때
		if (areaScript->GetJellyPushType() == m_JellyPushType && !m_SameType)
		{
			m_SameType = true;
			npcScript->TextClear();
			npcScript->SetText(L"헤롱 헤롱...");
		}
		// 아닐 때
		else if (m_SameType)
		{
			m_SameType = false;
			// 정신차리는 애니메이션 뒤 idle 애니메이션 
			npcScript->TextClear();
			npcScript->SetText(L"저 뒤에 있는 표지판... 아래쪽이 다 부서줬네...\n다른 곳에는 멀쩡한 표지판이 있을거야...");
		}
	}
}

void CBugNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 꽃의 빛이 사라졌을 때
	CFlowerLightAreaScript* areaScript = dynamic_cast<CFlowerLightAreaScript*>(_OtherObject->GetScript("CFlowerLightAreaScript"));
	if (areaScript != nullptr)
	{
		m_SameType = false;
		CNPCScript* npcScript = dynamic_cast<CNPCScript*>(GetOwner()->GetScript("CNPCScript"));
		npcScript->TextClear();
		npcScript->SetText(L"저 뒤에 있는 표지판... 아래쪽이 다 부서줬네...\n다른 곳에는 멀쩡한 표지판이 있을거야...");
	}
}

void CBugNPCScript::SaveToFile(FILE* _File)
{
	fwrite(&m_JellyPushType, sizeof(JellyPushType), 1, _File);
}

void CBugNPCScript::LoadFromFile(FILE* _File)
{
	fread(&m_JellyPushType, sizeof(JellyPushType), 1, _File);
}
