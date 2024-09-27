#include "pch.h"
#include "CSymbolScript.h"

CSymbolScript::CSymbolScript()
	: CScript(UINT(SCRIPT_TYPE::SYMBOLSCRIPT))
	, m_SaveTime(0.f)
	, m_Alpha(0.7f)
{
}

CSymbolScript::CSymbolScript(const CSymbolScript& _Origin)
	: CScript(_Origin)
	, m_SaveTime(0.f)
	, m_Alpha(0.7f)
{
}

CSymbolScript::~CSymbolScript()
{
}

void CSymbolScript::Begin()
{
	// ���� ��ġ�� ����Ʈ ��ȯ
	// ���� DownEffect ����Ʈ�� �ƴ����� ã�µ� �ð��� �ɷ� �ӽ÷� �̰�...
	Ptr<CPrefab> Effect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\DownEffect.pref");
	CGameObject* EffectObj = Instantiate(Effect, 0, GetOwner()->Transform()->GetRelativePos(), L"DownEffect");
	EffectObj->Transform()->SetRelativeScale(Vec3(600.f, 600.f, 1.f));
}

void CSymbolScript::Tick()
{
	m_SaveTime += DT;

	if (m_SaveTime <= 1.f)
	{
		// Ŀ����
		Vec3 scale = GetOwner()->Transform()->GetRelativeScale();
		scale.x = 300.f + (500.f - 300.f) * (m_SaveTime / 1.f);
		scale.y = 300.f + (500.f - 300.f) * (m_SaveTime / 1.f);
		scale.z = -2000.f;
		GetOwner()->Transform()->SetRelativeScale(scale);
	}
	else
	{
		// �������.
		m_Alpha = 0.7f + (0.f - 0.7f) * ((m_SaveTime - 1.f) / 1.5f);
		GetOwner()->SpriteComponent()->AddAlpha(m_Alpha);

		if (m_Alpha <= 0.f)
		{
			DeleteObject(GetOwner());
		}
	}
}

void CSymbolScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSymbolScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSymbolScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CSymbolScript::SaveToFile(FILE* _File)
{
}

void CSymbolScript::LoadFromFile(FILE* _File)
{
}
