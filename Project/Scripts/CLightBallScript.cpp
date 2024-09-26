#include "pch.h"
#include "CLightBallScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include <random>

#include "CPlayerScript.h"

CLightBallScript::CLightBallScript()
	: CScript(UINT(SCRIPT_TYPE::LIGHTBALLSCRIPT))
	, m_Time(0.f)
	, m_Speed(200.f)
	, m_Destroy(false)
{
}

CLightBallScript::CLightBallScript(const CLightBallScript& _Origin)
	: CScript(_Origin)
	, m_Time(0.f)
	, m_Speed(200.f)
	, m_Destroy(false)
{
}

CLightBallScript::~CLightBallScript()
{
}

void CLightBallScript::Begin()
{
	// 레이어 바꾸기
	int LayerIdx = GetOwner()->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);

	pLayer->LayerChange(GetOwner(), 10);

	// 속도 랜덤하게 주기
	static std::mt19937 engine(static_cast<unsigned int>(std::time(nullptr)));
	std::uniform_int_distribution<int> dist(100, 400);
	int random = dist(engine);

	m_Speed = (float)random;
}

void CLightBallScript::Tick()
{
	m_Time += DT;

	if (m_Destroy)
	{
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			DeleteObject(GetOwner());
		}
		return;
	}

	if (m_Time >= 4.5f)
	{
		Destroy();
	}

	Vec3 up = GetOwner()->Transform()->GetRelativeDir(DIR::UP);
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();

	pos += m_Speed * DT * up;

	GetOwner()->Transform()->SetRelativePos(pos);
}

void CLightBallScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 플레이어에게 Hit보내기
	if (_OtherObject->GetLayerIdx() == 3)
	{
		if (_OtherObject->GetScript("CPlayerScript") != nullptr)
		{
			CPlayerScript* script = dynamic_cast<CPlayerScript*>(_OtherObject->GetScript("CPlayerScript"));
			script->Hit();

			Destroy();
		}
	}
	// 벽, 같은 공격에 부딪치면 튕겨짐
	else if (_OtherObject->GetLayerIdx() == 28 || _OtherObject->GetLayerIdx() == 29 || _OtherObject->GetLayerIdx() == 6 || _OtherObject->GetLayerIdx() == 10)
	{
		// 반사
		Vec3 up = GetOwner()->Transform()->GetRelativeDir(DIR::UP);
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();

		pos += m_Speed * 2.f * DT * -up;

		GetOwner()->Transform()->SetRelativePos(pos);

		// 회전
		Vec3 rot = GetOwner()->Transform()->GetRelativeRotation();
		rot.z += 30.f;
		GetOwner()->Transform()->SetRelativeRotation(rot);
	}
}

void CLightBallScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 28 || _OtherObject->GetLayerIdx() == 29 || _OtherObject->GetLayerIdx() == 6 || _OtherObject->GetLayerIdx() == 10)
	{
		// 반사
		Vec3 up = GetOwner()->Transform()->GetRelativeDir(DIR::UP);
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();

		pos += m_Speed * 2.f * DT * -up;

		GetOwner()->Transform()->SetRelativePos(pos);
	}
}

void CLightBallScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLightBallScript::SaveToFile(FILE* _File)
{
}

void CLightBallScript::LoadFromFile(FILE* _File)
{
}

void CLightBallScript::Destroy()
{
	GetOwner()->FlipBookComponent()->Play(0, 10, false);
	GetOwner()->FlipBookComponent()->AddColor(false);

	for (auto i : GetOwner()->GetChildren())
	{
		DeleteObject(i);
	}

	m_Destroy = true;
}
