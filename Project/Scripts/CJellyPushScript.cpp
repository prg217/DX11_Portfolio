#include "pch.h"
#include "CJellyPushScript.h"

CJellyPushScript::CJellyPushScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHSCRIPT))
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_Type);
}

CJellyPushScript::CJellyPushScript(const CJellyPushScript& _Origin)
	: CScript(_Origin)
{
}

CJellyPushScript::~CJellyPushScript()
{
}

void CJellyPushScript::Begin()
{
}

void CJellyPushScript::Tick()
{
}

void CJellyPushScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// �ִ� ���ۿ� ������� �ڵ����� ����� ����

	// �̴� ���� �� ���
	// ���� ���̸� ���� �з���
	// �ٸ� ���̸� �� ���� ���� �ռ���(���� ��ġ ��� �������� ���ٰ� ������)

	// ū ���� �� ���
	// ��� �и��Ǿ� ���� 1���� �÷��̾ ��� ��
}

void CJellyPushScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushScript::SaveToFile(FILE* _File)
{
}

void CJellyPushScript::LoadFromFile(FILE* _File)
{
}
