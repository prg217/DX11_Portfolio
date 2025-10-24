#include "pch.h"
#include "CBugNPCScript.h"

#include "CJellyPushScript.h"
#include "CFlowerLightAreaScript.h"
#include "CNPCScript.h"

CBugNPCScript::CBugNPCScript()
	: CScript(UINT(SCRIPT_TYPE::BUGNPCSCRIPT))
	, m_JellyPushType(JellyPushType::NONE)
	, m_SameType(false)
	, m_Happy(false)
	, m_WakeUp(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_JellyPushType);
}

CBugNPCScript::CBugNPCScript(const CBugNPCScript& _Origin)
	: CScript(_Origin)
	, m_JellyPushType(_Origin.m_JellyPushType)
	, m_SameType(false)
	, m_Happy(false)
	, m_WakeUp(false)
{
}

CBugNPCScript::~CBugNPCScript()
{
}

void CBugNPCScript::Begin()
{
	m_npcScript = dynamic_cast<CNPCScript*>(GetOwner()->GetScript("CNPCScript"));

	m_npcScript->SetName(L"벌레 주민");
}

void CBugNPCScript::Tick()
{
	if (m_Happy)
	{
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			GetOwner()->FlipBookComponent()->Play(2, 10, true);
			m_Happy = false;
		}
	}
	if (m_WakeUp)
	{
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			GetOwner()->FlipBookComponent()->Play(2, 10, true);
			m_WakeUp = false;
		}
	}
}

void CBugNPCScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 애니메이션 : 0 헤롱 1 정신차림 2 평상시 3 기쁨

	// 플레이어 상호작용에 닿으면
	if (_OtherObject->GetLayerIdx() == 5)
	{
		// 헤롱헤롱 상태일 때는 return
		if (m_SameType)
		{
			return;
		}

		GetOwner()->FlipBookComponent()->Play(3, 10, false);
		m_Happy = true;
	}
}

void CBugNPCScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 꽃의 빛에 닿으면
	if (_OtherObject->GetLayerIdx() == 8) // 감지 레이어
	{
		CFlowerLightAreaScript* areaScript = dynamic_cast<CFlowerLightAreaScript*>(_OtherObject->GetScript("CFlowerLightAreaScript"));
		if (areaScript != nullptr)
		{
			// 꽃의 빛 색이 내 타입과 같을 때
			if (areaScript->GetJellyPushType() == m_JellyPushType && !m_SameType)
			{
				m_SameType = true;
				m_npcScript->TextClear();
				m_npcScript->LoadText(L"BugNPC_dizzy");

				GetOwner()->FlipBookComponent()->Play(0, 10, true);
			}
			// 아닐 때
			else if (areaScript->GetJellyPushType() != m_JellyPushType && m_SameType)
			{
				m_SameType = false;
				// 정신차리는 애니메이션 뒤 idle 애니메이션 
				m_npcScript->TextClear();
				m_npcScript->LoadText(L"BugNPC_idle");

				GetOwner()->FlipBookComponent()->Play(1, 10, false);
				m_WakeUp = true;
			}
		}
	}
}

void CBugNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 꽃의 빛이 사라졌을 때
	CFlowerLightAreaScript* areaScript = dynamic_cast<CFlowerLightAreaScript*>(_OtherObject->GetScript("CFlowerLightAreaScript"));
	if (areaScript != nullptr && m_SameType)
	{
		m_SameType = false;
		CNPCScript* npcScript = dynamic_cast<CNPCScript*>(GetOwner()->GetScript("CNPCScript"));
		npcScript->TextClear();
		m_npcScript->LoadText(L"BugNPC_idle");

		GetOwner()->FlipBookComponent()->Play(1, 10, false);
		m_WakeUp = true;
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
