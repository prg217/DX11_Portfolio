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
	// 본인 위치에 이펙트 소환
	// 본래 DownEffect 이펙트는 아니지만 찾는데 시간이 걸려 임시로 이걸...
	Ptr<CPrefab> Effect = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\DownEffect.pref");
	CGameObject* EffectObj = Instantiate(Effect, 0, GetOwner()->Transform()->GetRelativePos(), L"DownEffect");
	EffectObj->Transform()->SetRelativeScale(Vec3(600.f, 600.f, 1.f));
}

void CSymbolScript::Tick()
{
	m_SaveTime += DT;

	if (m_SaveTime <= 1.f)
	{
		// 커진다
		Vec3 scale = GetOwner()->Transform()->GetRelativeScale();
		scale.x = 300.f + (500.f - 300.f) * (m_SaveTime / 1.f);
		scale.y = 300.f + (500.f - 300.f) * (m_SaveTime / 1.f);
		scale.z = -2000.f;
		GetOwner()->Transform()->SetRelativeScale(scale);
	}
	else
	{
		// 흐려진다.
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
