#include "pch.h"
#include "CMonsterScript.h"

#include "CPlayerDetectScript.h"
#include "CHPScript.h"
#include "CSwingObjScript.h"

CMonsterScript::CMonsterScript()
	: CScript(UINT(SCRIPT_TYPE::MONSTERSCRIPT))
	, m_HPBar(nullptr)
	, m_Hit(false)
{
}

CMonsterScript::CMonsterScript(const CMonsterScript& _Origin)
	: CScript(_Origin)
	, m_HPBar(nullptr)
	, m_Hit(false)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::Begin()
{
	// 플레이어 감지 오브젝트 만들어서 자식에 넣기
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Player_Detect");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CPlayerDetectScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

	pObj->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pObj->Collider2D()->SetScale(Vec3(0.25f, 0.15f, 1.f));

	CPlayerDetectScript* script = dynamic_cast<CPlayerDetectScript*>(pObj->GetScript("CPlayerDetectScript"));
	script->SetMonsterScript(this);

	CreateObject(pObj, 8);
	AddChildObject(GetOwner(), pObj);

	// 자식에 있는 HPBar를 찾아서 저장
	for (auto i : GetOwner()->GetChildren())
	{
		if (i->GetName().compare(L"HPFrame") == 0)
		{
			for (auto j : i->GetChildren())
			{
				if (j->GetName().compare(L"HPBar") == 0)
				{
					m_HPBar = j;
				}
			}
			break;
		}
	}
}

void CMonsterScript::Tick()
{
}

void CMonsterScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CMonsterScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 물리적 충돌 되게 하기

	// 플레이어가 채 휘두르기 중일 때 채 휘두르기 콜라이더랑 만나면 데미지
	if (_OtherObject->GetLayerIdx() == 7)
	{
		CScript* script = _OtherObject->GetScript("CSwingObjScript");
		CSwingObjScript* pSwing = dynamic_cast<CSwingObjScript*>(script);
		if (pSwing)
		{
			if (pSwing->GetIsSwing() && !m_Hit)
			{
				m_Hit = true;
				Hit();
			}
			else if (!pSwing->GetIsSwing())
			{
				m_Hit = false;
			}
		}
	}
}

void CMonsterScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 7)
	{
		CScript* script = _OtherObject->GetScript("CSwingObjScript");
		CSwingObjScript* pSwing = dynamic_cast<CSwingObjScript*>(script);
		if (pSwing)
		{
			if (!pSwing->GetIsSwing())
			{
				m_Hit = false;
			}
		}
	}
}

void CMonsterScript::SaveToFile(FILE* _File)
{
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
}

void CMonsterScript::PlayerDetect(bool _In)
{
	if (_In)
	{
		// 공격 태세
	}
	else
	{
		// 멈춤
	}
}

void CMonsterScript::Hit()
{
	// 넉백 구현 필요(넉백되는 얘들만)

	// 타격 이펙트 추가 필요

	// 데미지 주기
	CHPScript* hpScript = dynamic_cast<CHPScript*>(GetOwner()->GetScript("CHPScript"));
	if (hpScript != nullptr)
	{
		hpScript->Hit(1, m_HPBar);
	}
}
