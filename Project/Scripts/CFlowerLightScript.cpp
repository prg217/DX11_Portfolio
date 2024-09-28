#include "pch.h"
#include "CFlowerLightScript.h"

#include "CJellyPushScript.h"
#include "CFlowerLightAreaScript.h"

CFlowerLightScript::CFlowerLightScript()
	: CScript(UINT(SCRIPT_TYPE::FLOWERLIGHTSCRIPT))
{
}

CFlowerLightScript::CFlowerLightScript(const CFlowerLightScript& _Origin)
	: CScript(_Origin)
{
}

CFlowerLightScript::~CFlowerLightScript()
{
}

void CFlowerLightScript::Begin()
{
}

void CFlowerLightScript::Tick()
{
}

void CFlowerLightScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CFlowerLightScript::SaveToFile(FILE* _File)
{
}

void CFlowerLightScript::LoadFromFile(FILE* _File)
{
}

void CFlowerLightScript::Bloom(JellyPushType _JellyPushType)
{
	if (_JellyPushType != m_Type)
	{
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_78_FlowerLightStandOn.wav");
		pSound->Play(1, 1.f, false);
	}
	m_Type = _JellyPushType;
	GetOwner()->FlipBookComponent()->Play(0, 8, false);

	for (auto i : GetOwner()->GetChildren())
	{
		if (wcscmp(i->GetName().c_str(), L"FlowerLight_Area") == 0)
		{
			i->Transform()->SetRelativePos(Vec3(0, -0.5f, 0));
			// 색 적용 시키기
			CFlowerLightAreaScript* areaScript = dynamic_cast<CFlowerLightAreaScript*>(i->GetScript("CFlowerLightAreaScript"));
			if (areaScript != nullptr)
			{
				areaScript->SetJellyPushType(m_Type);
			}
		}
	}
}

void CFlowerLightScript::Fall()
{
	GetOwner()->FlipBookComponent()->ReversePlay(0, 8, false);

	for (auto i : GetOwner()->GetChildren())
	{
		if (wcscmp(i->GetName().c_str(), L"FlowerLight_Area") == 0)
		{
			i->Transform()->SetRelativePos(Vec3(0, 15, 0));
		}
	}
}
