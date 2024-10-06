#include "pch.h"
#include "CLightBallScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CPlayerScript.h"

#include <random>

CLightBallScript::CLightBallScript()
	: CScript(UINT(SCRIPT_TYPE::LIGHTBALLSCRIPT))
	, m_Time(0.f)
	, m_Speed(200.f)
	, m_Destroy(false)
	, m_Particle (nullptr)
	, m_DeadOK(true)
{
}

CLightBallScript::CLightBallScript(const CLightBallScript& _Origin)
	: CScript(_Origin)
	, m_Time(0.f)
	, m_Speed(200.f)
	, m_Destroy(false)
	, m_Particle(nullptr)
	, m_DeadOK(_Origin.m_DeadOK)
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

	// 파티클 소환
	m_Particle = new CGameObject;
	m_Particle->SetName(L"Particle");

	m_Particle->AddComponent(new CTransform);
	m_Particle->AddComponent(new CParticleSystem);

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"particle\\boss_lightBall.particle";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	m_Particle->ParticleSystem()->LoadFromFile(File);
	fclose(File);

	// 파티클 회전 설정
	Vec3 rot = GetOwner()->Transform()->GetRelativeRotation();
	rot.z += ((90.f / 180.f) * XM_PI);
	m_Particle->Transform()->SetRelativeRotation(rot);

	m_Particle->Transform()->SetRelativePos(Vec3(0, -0.2f, 0));

	CreateObject(m_Particle, 0);
	AddChildObject(GetOwner(), m_Particle);
}

void CLightBallScript::Tick()
{
	m_Time += DT;

	if (m_Destroy)
	{
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			GetOwner()->FlipBookComponent()->AddAlpha(0.f);
			DeleteComponent(GetOwner(), COMPONENT_TYPE::COLLIDER2D);
			DeleteComponent(GetOwner(), COMPONENT_TYPE::LIGHT2D);

			if (m_DeadOK)
			{
				DeleteObject(GetOwner());
			}
		}
		return;
	}

	if (m_Time >= 4.5f && m_DeadOK)
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

		pos += m_Speed * 5.f * DT * -up;

		GetOwner()->Transform()->SetRelativePos(pos);

		// 회전
		std::random_device rd;  // 시드로 사용할 랜덤 장치
		std::mt19937 gen(rd()); // 난수 생성 엔진
		std::uniform_real_distribution<float> dis(60.f, 120.f);

		Vec3 rot = GetOwner()->Transform()->GetRelativeRotation();
		rot.z += ((dis(gen) / 180.f) * XM_PI);
		GetOwner()->Transform()->SetRelativeRotation(rot);
		if (m_Particle != nullptr)
		{
			Vec3 rot2 = GetOwner()->Transform()->GetRelativeRotation();
			rot2.z += ((dis(gen) / 180.f) * XM_PI);
			m_Particle->Transform()->SetRelativeRotation(rot2);
		}
	}
}

void CLightBallScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 28 || _OtherObject->GetLayerIdx() == 29 || _OtherObject->GetLayerIdx() == 6 || _OtherObject->GetLayerIdx() == 10)
	{
		// 반사
		Vec3 up = GetOwner()->Transform()->GetRelativeDir(DIR::UP);
		Vec3 pos = GetOwner()->Transform()->GetRelativePos();

		pos += m_Speed * 2.5f * DT * -up;

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
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_97_BugBulletDisappear.wav");
	pSound->Play(1, 1.f, false);

	GetOwner()->FlipBookComponent()->Play(0, 10, false);
	GetOwner()->FlipBookComponent()->AddColor(false);

	for (auto i : GetOwner()->GetChildren())
	{
		DeleteObject(i);
	}
	m_Particle = nullptr;
	m_Destroy = true;
}
