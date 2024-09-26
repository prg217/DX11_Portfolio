#include "pch.h"
#include "CBugBallScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CPlayerScript.h"
#include "CBugBossScript.h"
#include "CSwingObjScript.h"

CBugBallScript::CBugBallScript()
	: CScript(UINT(SCRIPT_TYPE::BUGBALLSCRIPT))
	, m_Boss(nullptr)
	, m_Time(0.f)
	, m_Speed(200.f)
	, m_Destroy(false)
	, m_Particle(nullptr)
	, m_Hit(false)
{
}

CBugBallScript::CBugBallScript(const CBugBallScript& _Origin)
	: CScript(_Origin)
	, m_Boss(nullptr)
	, m_Time(0.f)
	, m_Speed(200.f)
	, m_Destroy(false)
	, m_Particle(nullptr)
	, m_Hit(false)
{
}

CBugBallScript::~CBugBallScript()
{
}

void CBugBallScript::Begin()
{
	// ��ȯ ���� �� ������.
	GetOwner()->FlipBookComponent()->AddColor(true, Vec3(1.f, 1.f, 1.f));

	// ���̾� �ٲٱ�
	int LayerIdx = GetOwner()->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);

	pLayer->LayerChange(GetOwner(), 4);

	// ��ƼŬ ��ȯ
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

	// ��ƼŬ ȸ�� ����
	Vec3 rot = GetOwner()->Transform()->GetRelativeRotation();
	rot.z += ((90.f / 180.f) * XM_PI);
	m_Particle->Transform()->SetRelativeRotation(rot);

	m_Particle->Transform()->SetRelativePos(Vec3(0, -0.2f, 0));

	CreateObject(m_Particle, 0);
	AddChildObject(GetOwner(), m_Particle);

	// ���� ���
	m_Boss = pCurLevel->FindObjectByName(L"BugBoss");
}

void CBugBallScript::Tick()
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
	else if (m_Time >= 0.2f)
	{
		GetOwner()->FlipBookComponent()->AddColor(false);
	}

	Vec3 up = GetOwner()->Transform()->GetRelativeDir(DIR::UP);
	Vec3 pos = GetOwner()->Transform()->GetRelativePos();

	pos += m_Speed * DT * up;

	GetOwner()->Transform()->SetRelativePos(pos);
}

void CBugBallScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (!m_Hit)
	{
		// �÷��̾�� Hit������
		if (_OtherObject->GetLayerIdx() == 3)
		{
			if (_OtherObject->GetScript("CPlayerScript") != nullptr)
			{
				CPlayerScript* script = dynamic_cast<CPlayerScript*>(_OtherObject->GetScript("CPlayerScript"));
				script->Hit();

				Destroy();
			}
		}
		// ���� �ε�ġ�� ����
		else if (_OtherObject->GetLayerIdx() == 28 || _OtherObject->GetLayerIdx() == 29 || _OtherObject->GetLayerIdx() == 6 || _OtherObject->GetLayerIdx() == 10)
		{
			Destroy();
		}
	}

	// �������� Hit������
	if (_OtherObject->GetLayerIdx() == 4)
	{
		if (_OtherObject->GetScript("CBugBossScript") != nullptr)
		{
			CBugBossScript* script = dynamic_cast<CBugBossScript*>(_OtherObject->GetScript("CBugBossScript"));
			script->Hit();

			Destroy();
		}
	}
}

void CBugBallScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// �÷��̾ ä �ֵθ��� ���� �� ä �ֵθ��� �ݶ��̴��� ������ �������� ������.
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
		}
	}
}

void CBugBallScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBugBallScript::SaveToFile(FILE* _File)
{
}

void CBugBallScript::LoadFromFile(FILE* _File)
{
}

void CBugBallScript::Destroy()
{
	GetOwner()->FlipBookComponent()->Play(1, 10, false, 1);
	GetOwner()->FlipBookComponent()->AddColor(false);

	for (auto i : GetOwner()->GetChildren())
	{
		DeleteObject(i);
	}

	m_Destroy = true;
}

void CBugBallScript::Hit()
{
	if (m_Boss == nullptr)
	{
		return;
	}

	// �����ٰ� ���� �������� ȸ�� �Ѵ�.
	GetOwner()->FlipBookComponent()->AddColor(true, Vec3(1.f, 1.f, 1.f));
	m_Time = 0.f;

	Vec3 pos = GetOwner()->Transform()->GetRelativePos();
	Vec3 bossPos = m_Boss->Transform()->GetRelativePos();

	Vec2 vDirection = Vec2(bossPos.x - pos.x, bossPos.y - pos.y);
	float angleZ = atan2(vDirection.y, vDirection.x);

	angleZ -= XM_PIDIV2; // 90�� ����

	GetOwner()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, angleZ));
}