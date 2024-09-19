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
	// �ִϸ��̼� : 0 ��� 1 �������� 2 ���� 3 ���

	// �÷��̾� ��ȣ�ۿ뿡 ������
	if (_OtherObject->GetLayerIdx() == 5)
	{
		// ������ ������ ���� return
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
	// ���� ���� ������
	if (_OtherObject->GetLayerIdx() == 8) // ���� ���̾�
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

				GetOwner()->FlipBookComponent()->Play(0, 10, true);
			}
			// �ƴ� ��
			else if (areaScript->GetJellyPushType() != m_JellyPushType && m_SameType)
			{
				m_SameType = false;
				// ���������� �ִϸ��̼� �� idle �ִϸ��̼� 
				npcScript->TextClear();
				npcScript->SetText(L"�� �ڿ� �ִ� ǥ����... �Ʒ����� �� �μ����...\n�ٸ� ������ ������ ǥ������ �����ž�...");

				GetOwner()->FlipBookComponent()->Play(1, 10, false);
				m_WakeUp = true;
			}
		}
	}
}

void CBugNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// ���� ���� ������� ��
	CFlowerLightAreaScript* areaScript = dynamic_cast<CFlowerLightAreaScript*>(_OtherObject->GetScript("CFlowerLightAreaScript"));
	if (areaScript != nullptr && m_SameType)
	{
		m_SameType = false;
		CNPCScript* npcScript = dynamic_cast<CNPCScript*>(GetOwner()->GetScript("CNPCScript"));
		npcScript->TextClear();
		npcScript->SetText(L"�� �ڿ� �ִ� ǥ����... �Ʒ����� �� �μ����...\n�ٸ� ������ ������ ǥ������ �����ž�...");

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
