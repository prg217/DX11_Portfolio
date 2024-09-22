#include "pch.h"
#include "CMonsterScript.h"

#include "CPlayerDetectScript.h"
#include "CHPScript.h"
#include "CSwingObjScript.h"

#include "CSpitCactusScript.h"

CMonsterScript::CMonsterScript()
	: CScript(UINT(SCRIPT_TYPE::MONSTERSCRIPT))
	, m_HPBar(nullptr)
	, m_HPFrame(nullptr)
	, m_HpScript(nullptr)
	, m_Hit(false)
	, m_SaveHitTime(0)
	, m_MonsterType(MonsterType::SpitCactus)
{
	AddScriptParam(SCRIPT_PARAM::INT, "MonsterType", &m_MonsterType);
}

CMonsterScript::CMonsterScript(const CMonsterScript& _Origin)
	: CScript(_Origin)
	, m_HPBar(nullptr)
	, m_HPFrame(nullptr)
	, m_HpScript(nullptr)
	, m_Hit(false)
	, m_SaveHitTime(0)
	, m_MonsterType(_Origin.m_MonsterType)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::Begin()
{
	m_HpScript = dynamic_cast<CHPScript*>(GetOwner()->GetScript("CHPScript"));

	// 플레이어 감지 오브젝트 만들어서 자식에 넣기
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Player_Detect");
	pObj->AddComponent(new CTransform);
	pObj->AddComponent(new CCollider2D);
	pObj->AddComponent(new CPlayerDetectScript);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pObj->Transform()->SetRelativeScale(Vec3(1.f, 1.f, 1.f));

	pObj->Collider2D()->SetIndependentScale(true);
	pObj->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pObj->Collider2D()->SetScale(Vec3(450.f, 450.f, 1.f));

	CPlayerDetectScript* script = dynamic_cast<CPlayerDetectScript*>(pObj->GetScript("CPlayerDetectScript"));
	script->SetMonsterScript(this);

	CreateObject(pObj, 8);
	AddChildObject(GetOwner(), pObj);

	// 자식에 있는 HPBar를 찾아서 저장
	for (auto i : GetOwner()->GetChildren())
	{
		if (i->GetName().compare(L"HPFrame") == 0)
		{
			m_HPFrame = i;
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
	// Hit가 일정 시간 동안 안 들어올 경우 안 보이게 한다.
	if (TIME - m_SaveHitTime >= 3.f)
	{
		if (m_HPFrame != nullptr)
		{
			Vec3 pos = m_HPFrame->Transform()->GetRelativePos();
			m_HPFrame->Transform()->SetRelativePos(pos.x, 30.f, pos.z);
		}
	}
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
	fwrite(&m_MonsterType, sizeof(MonsterType), 1, _File);
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
	fread(&m_MonsterType, sizeof(MonsterType), 1, _File);
}

void CMonsterScript::PlayerDetect(bool _In)
{
	if (_In)
	{
		// 공격 태세
		switch (m_MonsterType)
		{
		case MonsterType::SpitCactus:
		{
			CSpitCactusScript* pSpitCactusScript = dynamic_cast<CSpitCactusScript*>(GetOwner()->GetScript("CSpitCactusScript"));
			pSpitCactusScript->Attack();
		}
		break;
		default:
			break;
		}
	}
	else
	{
		// 멈춤
		switch (m_MonsterType)
		{
		case MonsterType::SpitCactus:
		{
			CSpitCactusScript* pSpitCactusScript = dynamic_cast<CSpitCactusScript*>(GetOwner()->GetScript("CSpitCactusScript"));
			pSpitCactusScript->Stop();
		}
		break;
		default:
			break;
		}
	}
}

void CMonsterScript::Hit()
{
	// 넉백 구현 필요(넉백되는 얘들만)

	// 피격 이펙트 추가 필요

	// 데미지 주기
	if (m_HpScript != nullptr)
	{
		m_SaveHitTime = TIME;
		Vec3 pos = m_HPFrame->Transform()->GetRelativePos();
		m_HPFrame->Transform()->SetRelativePos(pos.x, 0.3f, pos.z);
		m_HpScript->Hit(1, m_HPBar);
	}
}

void CMonsterScript::Dead()
{
	switch (m_MonsterType)
	{
	case MonsterType::SpitCactus:
	{
		CSpitCactusScript* pSpitCactusScript = dynamic_cast<CSpitCactusScript*>(GetOwner()->GetScript("CSpitCactusScript"));
		pSpitCactusScript->Dead();
	}
	break;
	default:
		break;
	}
}
