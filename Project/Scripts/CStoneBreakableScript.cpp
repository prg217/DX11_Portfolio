#include "pch.h"
#include "CStoneBreakableScript.h"

CStoneBreakableScript::CStoneBreakableScript()
	: CScript(UINT(SCRIPT_TYPE::STONEBREAKABLESCRIPT))
	, m_Destroy(false)
{
}

CStoneBreakableScript::CStoneBreakableScript(const CStoneBreakableScript& _Origin)
	: CScript(_Origin)
	, m_Destroy(false)
{
}

CStoneBreakableScript::~CStoneBreakableScript()
{
}

void CStoneBreakableScript::Begin()
{
}

void CStoneBreakableScript::Tick()
{
	if (m_Destroy)
	{
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			DeleteObject(GetOwner());
		}
	}
}

void CStoneBreakableScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CStoneBreakableScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CStoneBreakableScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CStoneBreakableScript::SaveToFile(FILE* _File)
{
}

void CStoneBreakableScript::LoadFromFile(FILE* _File)
{
}

void CStoneBreakableScript::Destroy()
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_681_StoneBreakableDestroy.wav");
	pSound->Play(1, 1.f, true);

	m_Destroy = true;

	GetOwner()->FlipBookComponent()->Play(0, 10, false);
}
