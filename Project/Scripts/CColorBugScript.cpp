#include "pch.h"
#include "CColorBugScript.h"

#include <random>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CBugBossScript.h"
#include "CHPScript.h"

CColorBugScript::CColorBugScript()
	: CScript(UINT(SCRIPT_TYPE::COLORBUGSCRIPT))
	, m_Type(ColorBugType::Blue)
	, m_RotObj(nullptr)
	, m_SaveTime(0.f)
	, m_Speed(150.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Type", &m_Type);
}

CColorBugScript::CColorBugScript(const CColorBugScript& _Origin)
	: CScript(_Origin)
	, m_Type(_Origin.m_Type)
	, m_RotObj(nullptr)
	, m_SaveTime(0.f)
	, m_Speed(_Origin.m_Speed)
{
}

CColorBugScript::~CColorBugScript()
{
}

void CColorBugScript::Begin()
{
	CHPScript* hpScript = dynamic_cast<CHPScript*>(GetOwner()->GetScript("CHPScript"));
	hpScript->SetMaxHP(2);
	hpScript->SetHP(2);

	// 레이어 바꾸기
	int LayerIdx = GetOwner()->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);

	pLayer->LayerChange(GetOwner(), 4);

	for (auto i : GetOwner()->GetChildren())
	{
		if (wcscmp(i->GetName().c_str(), L"Rot") == 0)
		{
			m_RotObj = i;
		}

		// 빛 적용 끄기
		if (wcscmp(i->GetName().c_str(), L"HPFrame") == 0)
		{
			i->SpriteComponent()->SetUseLight(false);
			for (auto j : i->GetChildren())
			{
				if (wcscmp(i->GetName().c_str(), L"HPBG") == 0)
				{
					j->SpriteComponent()->SetUseLight(false);
				}
				if (wcscmp(i->GetName().c_str(), L"HPBar") == 0)
				{
					j->SpriteComponent()->SetUseLight(false);
				}
			}
		}
	}
}

void CColorBugScript::Tick()
{
	// 상하좌우, 대각선으로 랜덤하게 이동
	m_SaveTime += DT;

	if (m_SaveTime >= 3.f)
	{
		float rotZ = GetOwner()->Transform()->GetRelativeRotation().z;

		// 45도 단위로 랜덤하게 돈다.
		std::random_device rd;
		std::mt19937 gen(rd()); 
		std::uniform_int_distribution<> distrib(0, 7);

		rotZ += distrib(gen) * 45.f;
		rotZ = fmod(rotZ, 360.0f);
		m_RotObj->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, ((rotZ / 180.f) * XM_PI)));

		m_SaveTime = 0.f;
	}

	Vec3 up = m_RotObj->Transform()->GetRelativeDir(DIR::UP);
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();

	pos += m_Speed * DT * up;
	pos.z = pos.y;
	GetOwner()->Transform()->SetRelativePos(pos);
}

void CColorBugScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CColorBugScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 벽에 부딪치면 못 나감
	if (_OtherObject->GetLayerIdx() == 28 || _OtherObject->GetLayerIdx() == 29 || _OtherObject->GetLayerIdx() == 6 || _OtherObject->GetLayerIdx() == 10 || _OtherObject->GetLayerIdx() == 4)
	{
		// 보스 예외
		if (wcscmp(_OtherObject->GetName().c_str(), L"BugBoss") == 0)
		{
			return;
		}

		Vec3 up = m_RotObj->Transform()->GetRelativeDir(DIR::UP);
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();

		pos += m_Speed * DT * -up;

		GetOwner()->Transform()->SetRelativePos(pos);
	}
}

void CColorBugScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CColorBugScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(ColorBugType), 1, _File);
}

void CColorBugScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(ColorBugType), 1, _File);
}

void CColorBugScript::Dead()
{
	// 죽는 이펙트

	// BugBoss에게 신호 보냄
	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CGameObject* boss = curLevel->FindObjectByName(L"BugBoss");
	CBugBossScript* bossScript = dynamic_cast<CBugBossScript*>(boss->GetScript("CBugBossScript"));
	bossScript->Phase23Down(m_Type);

	DeleteObject(GetOwner());
}
