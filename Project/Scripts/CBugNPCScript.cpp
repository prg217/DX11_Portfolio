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
		// ���� �� ���� �� Ÿ�԰� ���� ��
		if (areaScript->GetJellyPushType() == m_JellyPushType && !m_SameType)
		{
			m_SameType = true;
			npcScript->TextClear();
			npcScript->SetText(L"��� ���...");
		}
		// �ƴ� ��
		else if (m_SameType)
		{
			m_SameType = false;
			// ���������� �ִϸ��̼� �� idle �ִϸ��̼� 
			npcScript->TextClear();
			npcScript->SetText(L"�� �ڿ� �ִ� ǥ����... �Ʒ����� �� �μ����...\n�ٸ� ������ ������ ǥ������ �����ž�...");
		}
	}
}

void CBugNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// ���� ���� ������� ��
	CFlowerLightAreaScript* areaScript = dynamic_cast<CFlowerLightAreaScript*>(_OtherObject->GetScript("CFlowerLightAreaScript"));
	if (areaScript != nullptr)
	{
		m_SameType = false;
		CNPCScript* npcScript = dynamic_cast<CNPCScript*>(GetOwner()->GetScript("CNPCScript"));
		npcScript->TextClear();
		npcScript->SetText(L"�� �ڿ� �ִ� ǥ����... �Ʒ����� �� �μ����...\n�ٸ� ������ ������ ǥ������ �����ž�...");
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
