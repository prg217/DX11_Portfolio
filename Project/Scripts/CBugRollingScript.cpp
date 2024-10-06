#include "pch.h"
#include "CBugRollingScript.h"

#include "CJellyPushScript.h"

#include "CMonsterScript.h"
#include "CPlayerScript.h"
#include "CFlowerLightAreaScript.h"

#include "CPlayerDetectScript.h"
#include "CAniFinishDestroyScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CBugRollingScript::CBugRollingScript()
	: CScript(UINT(SCRIPT_TYPE::BUGROLLINGSCRIPT))
	, m_pMonsterScript(nullptr)
	, m_pPlayer(nullptr)
	, m_RollParticle(nullptr)
	, m_Speed(200.f)
	, m_Attack(false)
	, m_End(false)
	, m_ChaseTime(0.f)
	, m_CurAni(BugRollingAni::IDLE_DOWN)
	, m_CurDir(BugRollingDir::NONE)
	, m_JellyType(JellyPushType::MAGENTA)
	, m_Stun(false)
	, m_StunObj(nullptr)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::GAMEOBJECT, "Player", &m_pPlayer);
}

CBugRollingScript::CBugRollingScript(const CBugRollingScript& _Origin)
	: CScript(_Origin)
	, m_pMonsterScript(nullptr)
	, m_pPlayer(_Origin.m_pPlayer)
	, m_RollParticle(nullptr)
	, m_Speed(_Origin.m_Speed)
	, m_Attack(false)
	, m_End(false)
	, m_ChaseTime(0.f)
	, m_CurAni(BugRollingAni::IDLE_DOWN)
	, m_CurDir(BugRollingDir::NONE)
	, m_JellyType(_Origin.m_JellyType)
	, m_Stun(false)
	, m_StunObj(nullptr)
{
}

CBugRollingScript::~CBugRollingScript()
{
}

void CBugRollingScript::Begin()
{
	m_pMonsterScript = dynamic_cast<CMonsterScript*>(GetOwner()->GetScript("CMonsterScript"));

	CLevel* curLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	m_pPlayer = curLevel->FindObjectByName(L"Player");

	for (auto i : GetOwner()->GetChildren())
	{
		if (i->GetName().compare(L"RollParticle"))
		{
			m_RollParticle = i;
		}
	}
}

void CBugRollingScript::Tick()
{
	if (m_End)
	{
		MoveEndAni();
	}

	// ���� Ȱ��ȭ
	if (m_Attack && !m_Stun)
	{
		// ������ ���߿��� �ǰ��� �Ұ����ϴ�.
		m_pMonsterScript->SetHitOK(false);

		// �ִϸ��̼� ����
		AniChange();

		// ������ ���°� �ƴϸ� return
		switch (m_CurAni)
		{
		case BugRollingAni::IDLE_DOWN:
		case BugRollingAni::IDLE_UP:
		case BugRollingAni::IDLE_LEFT:
		case BugRollingAni::IDLE_RIGHT:
		case BugRollingAni::DOWN:
		case BugRollingAni::UP:
		case BugRollingAni::LEFT:
		case BugRollingAni::RIGHT:
		case BugRollingAni::END_DOWN:
		case BugRollingAni::END_UP:
		case BugRollingAni::END_LEFT:
		case BugRollingAni::END_RIGHT:
			return;
			break;
		default:
			break;
		}

		m_ChaseTime += DT;
		// 3�ʸ��� �÷��̾��� ������ �˾Ƴ��� �� �������� ������.
		if (m_ChaseTime >= 3.f)
		{
			m_ChaseTime = 0.f;
			Chase();
		}
		Move();
	}
}

void CBugRollingScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// �÷��̾�� �ε�ĥ ������ ������, ���� �˹�
	if (_OtherObject->GetLayerIdx() == 3)
	{
		if (_OtherObject->GetScript("CPlayerScript") != nullptr)
		{
			CPlayerScript* script = dynamic_cast<CPlayerScript*>(_OtherObject->GetScript("CPlayerScript"));
			script->Hit();
		}

		// �˹��� ������ ����
		if (m_Attack)
		{
			Vec3 force = Vec3(0.f, 0.f, 0.f);
			float speed = 1800.f;

			switch (m_CurDir)
			{
			case BugRollingDir::DOWN:
				force = Vec3(0.f, -speed, -speed);
				break;
			case BugRollingDir::UP:
				force = Vec3(0.f, speed, speed);
				break;
			case BugRollingDir::LEFT:
				force = Vec3(-speed, 0.f, 0.f);
				break;
			case BugRollingDir::RIGHT:
				force = Vec3(speed, 0.f, 0.f);
				break;
			case BugRollingDir::LEFTDOWN:
				force = Vec3(-speed, -speed, -speed);
				break;
			case BugRollingDir::LEFTUP:
				force = Vec3(-speed, speed, speed);
				break;
			case BugRollingDir::RIGHTDOWN:
				force = Vec3(speed, -speed, -speed);
				break;
			case BugRollingDir::RIGHTUP:
				force = Vec3(speed, speed, speed);
				break;
			default:
				break;
			}

			GetOwner()->RigidBody()->SetForce(force);
		}
	}
}

void CBugRollingScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// ���� ���� ������
	if (_OtherObject->GetLayerIdx() == 8) // ���� ���̾�
	{
		CFlowerLightAreaScript* areaScript = dynamic_cast<CFlowerLightAreaScript*>(_OtherObject->GetScript("CFlowerLightAreaScript"));
		if (areaScript != nullptr)
		{
			// ���� �� ���� �� Ÿ�԰� ���� ��
			if (areaScript->GetJellyPushType() == m_JellyType && !m_Stun)
			{
				Stun();

				// ���� ����Ʈ
				StunEffect();

				// Ÿ�� ����
				m_pMonsterScript->SetHitOK(true);
			}
			// �ƴ϶��
			else if (areaScript->GetJellyPushType() != m_JellyType && m_Stun)
			{
				m_Stun = false;

				// ���� ����Ʈ ����
				StunDelete();
			}
		}
	}

	// �浹 ����
	if (_OtherObject->GetLayerIdx() == 7 || _OtherObject->GetLayerIdx() == 8)
	{
		return;
	}

	// ���� ���� �ƴϰų� ������ �ٲ��� �� �ȵ��� ���� return
	if (!m_Attack || m_ChaseTime <= 0.1f)
	{
		return;
	}
	// ���������� �浹�ǰ� �ϱ�
	Vec3 force = Vec3(0.f, 0.f, 0.f);
	float speed = 800.f;

	switch (m_CurDir)
	{
	case BugRollingDir::DOWN:
		force = Vec3(0.f, -speed, -speed);
		break;
	case BugRollingDir::UP:
		force = Vec3(0.f, speed, speed);
		break;
	case BugRollingDir::LEFT:
		force = Vec3(-speed, 0.f, 0.f);
		break;
	case BugRollingDir::RIGHT:
		force = Vec3(speed, 0.f, 0.f);
		break;
	case BugRollingDir::LEFTDOWN:
		force = Vec3(-speed, -speed, -speed);
		break;
	case BugRollingDir::LEFTUP:
		force = Vec3(-speed, speed, speed);
		break;
	case BugRollingDir::RIGHTDOWN:
		force = Vec3(speed, -speed, -speed);
		break;
	case BugRollingDir::RIGHTUP:
		force = Vec3(speed, speed, speed);
		break;
	default:
		break;
	}

	GetOwner()->RigidBody()->SetForce(force);
}

void CBugRollingScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// ���� ���� �������
	if (_OtherObject->GetLayerIdx() == 8) // ���� ���̾�
	{
		if (m_Stun)
		{
			m_Stun = false;

			// ���� ����Ʈ ����
			StunDelete();
		}
	}
}

void CBugRollingScript::SaveToFile(FILE* _File)
{
	fwrite(&m_JellyType, sizeof(JellyPushType), 1, _File);
}

void CBugRollingScript::LoadFromFile(FILE* _File)
{
	fread(&m_JellyType, sizeof(JellyPushType), 1, _File);
}

void CBugRollingScript::AniChange()
{
	// ��� ����->������ �غ�
	// ������ �غ�->������
	switch (m_CurAni)
	{
	case BugRollingAni::IDLE_DOWN:
		m_CurAni = BugRollingAni::DOWN;
		GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);
		break;
	case BugRollingAni::IDLE_UP:
		m_CurAni = BugRollingAni::UP;
		GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);
		break;
	case BugRollingAni::IDLE_LEFT:
		m_CurAni = BugRollingAni::LEFT;
		GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);
		break;
	case BugRollingAni::IDLE_RIGHT:
		m_CurAni = BugRollingAni::RIGHT;
		GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);
		break;
	case BugRollingAni::DOWN:
	case BugRollingAni::UP:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::ROLLING_DOWNUP;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
		}
		break;
	case BugRollingAni::LEFT:
	case BugRollingAni::RIGHT:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::ROLLING_LEFTRIGHT;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
		}
		break;
	default:
		break;
	}

	switch (m_CurAni)
	{
	case BugRollingAni::ROLLING_DOWNUP:
		switch (m_CurDir)
		{
		case BugRollingDir::LEFT:
		case BugRollingDir::RIGHT:
			m_CurAni = BugRollingAni::ROLLING_LEFTRIGHT;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
			break;
		default:
			break;
		}
		break;
	case BugRollingAni::ROLLING_LEFTRIGHT:
		switch (m_CurDir)
		{
		case BugRollingDir::DOWN:
		case BugRollingDir::UP:
			m_CurAni = BugRollingAni::ROLLING_DOWNUP;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CBugRollingScript::Chase()
{
	Vec3 playerPos = m_pPlayer->Transform()->GetRelativePos();
	Vec3 myPos = GetOwner()->Transform()->GetRelativePos();
	// ���� ���� ���
	Vec3 direction = playerPos - myPos;
	// ���� ���͸� ����ȭ
	direction.Normalize();

	// �밢��
	if (abs(abs(direction.x) - abs(direction.y)) < 0.25f)
	{
		if (direction.x > 0 && direction.y > 0)
		{
			// �ϵ�
			m_CurDir = BugRollingDir::RIGHTUP;
		}
		else if (direction.x < 0 && direction.y < 0)
		{
			// ���� 
			m_CurDir = BugRollingDir::LEFTDOWN;
		}
		else if (direction.x > 0 && direction.y < 0)
		{
			// ����
			m_CurDir = BugRollingDir::RIGHTDOWN;
		}
		else if (direction.x < 0 && direction.y > 0)
		{
			// �ϼ�
			m_CurDir = BugRollingDir::LEFTUP;
		}
	}
	else if (abs(direction.x) >= abs(direction.y))
	{
		if (direction.x > 0)
		{
			// ������
			m_CurDir = BugRollingDir::RIGHT;
		}
		else
		{
			// ����
			m_CurDir = BugRollingDir::LEFT;
		}
	}
	else
	{
		if (direction.y > 0)
		{
			// ��
			m_CurDir = BugRollingDir::UP;
		}
		else
		{
			// �Ʒ�
			m_CurDir = BugRollingDir::DOWN;
		}
	}
}

void CBugRollingScript::Move()
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_82_MonsterBugRolling_Rolling.wav");
	pSound->Play(1, 1.f, false);

	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	switch (m_CurDir)
	{
	case BugRollingDir::DOWN:
		vPos.y -= DT * m_Speed;
		vPos.z -= DT * m_Speed;
		break;
	case BugRollingDir::UP:
		vPos.y += DT * m_Speed;
		vPos.z += DT * m_Speed;
		break;
	case BugRollingDir::LEFT:
		vPos.x -= DT * m_Speed;
		break;
	case BugRollingDir::RIGHT:
		vPos.x += DT * m_Speed;
		break;
	case BugRollingDir::LEFTDOWN:
		vPos.x -= DT * m_Speed;
		vPos.y -= DT * m_Speed;
		vPos.z -= DT * m_Speed;
		break;
	case BugRollingDir::LEFTUP:
		vPos.x -= DT * m_Speed;
		vPos.y += DT * m_Speed;
		vPos.z += DT * m_Speed;
		break;
	case BugRollingDir::RIGHTDOWN:
		vPos.x += DT * m_Speed;
		vPos.y -= DT * m_Speed;
		vPos.z -= DT * m_Speed;
		break;
	case BugRollingDir::RIGHTUP:
		vPos.x += DT * m_Speed;
		vPos.y += DT * m_Speed;
		vPos.z += DT * m_Speed;
		break;
	default:
		break;
	}

	GetOwner()->Transform()->SetRelativePos(vPos);
}

void CBugRollingScript::MoveEndAni()
{
	// end �ִϸ��̼�
	switch (m_CurDir)
	{
	case BugRollingDir::DOWN:
	case BugRollingDir::LEFTDOWN:
	case BugRollingDir::RIGHTDOWN:
		m_CurAni = BugRollingAni::END_DOWN;
		break;
	case BugRollingDir::UP:
	case BugRollingDir::LEFTUP:
	case BugRollingDir::RIGHTUP:
		m_CurAni = BugRollingAni::END_UP;
		break;
	case BugRollingDir::LEFT:
		m_CurAni = BugRollingAni::END_LEFT;
		break;
	case BugRollingDir::RIGHT:
		m_CurAni = BugRollingAni::END_RIGHT;
		break;
	default:
		break;
	}
	GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, false);

	m_CurDir = BugRollingDir::NONE;

	// idle���·� �ǵ�����
	switch (m_CurAni)
	{
	case BugRollingAni::END_DOWN:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::IDLE_DOWN;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
			m_End = false;
		}
		break;
	case BugRollingAni::END_UP:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::IDLE_UP;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
			m_End = false;
		}
		break;
	case BugRollingAni::END_LEFT:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::IDLE_LEFT;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
			m_End = false;
		}
		break;
	case BugRollingAni::END_RIGHT:
		if (GetOwner()->FlipBookComponent()->GetIsFinish())
		{
			m_CurAni = BugRollingAni::IDLE_RIGHT;
			GetOwner()->FlipBookComponent()->Play((int)m_CurAni, 10, true);
			m_End = false;
		}
		break;
	default:
		break;
	}
}

void CBugRollingScript::Stun()
{
	m_Stun = true;
	m_End = true;

	// ��ƼŬ ��Ȱ��ȭ
	if (m_RollParticle != nullptr)
	{
		m_RollParticle->Transform()->SetRelativePos(Vec3(0, 30, 0));
	}
}

void CBugRollingScript::StunEffect()
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_82_MonsterBugRolling_Rolling.wav");
	pSound->Stop();

	if (GetOwner()->IsDead())
	{
		return;
	}

	m_StunObj = new CGameObject;
	m_StunObj->AddComponent(new CTransform);
	m_StunObj->AddComponent(new CMeshRender);
	m_StunObj->AddComponent(new CFlipBookComponent);

	m_StunObj->Transform()->SetRelativePos(Vec3(0, 0.2f, 0));
	m_StunObj->Transform()->SetRelativeScale(Vec3(1, 1, 1));
	
	m_StunObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_StunObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));
	
	Ptr<CFlipBook> pFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Effect\\stun.flip");
	m_StunObj->FlipBookComponent()->AddFlipBook(0, pFlip);
	m_StunObj->FlipBookComponent()->Play(0, 10, true);

	CreateObject(m_StunObj, 0);
	AddChildObject(GetOwner(), m_StunObj);
}

void CBugRollingScript::StunDelete()
{
	if (m_StunObj != nullptr)
	{
		DeleteObject(m_StunObj);

		m_StunObj = nullptr;
	}
}

void CBugRollingScript::Attack()
{
	// ���� Ȱ��ȭ
	m_Attack = true;
	Chase();

	// ������ ���� ��ƼŬ Ȱ��ȭ
	if (m_RollParticle != nullptr)
	{
		m_RollParticle->Transform()->SetRelativePos(Vec3(0, 0, 0));
	}
}

void CBugRollingScript::Stop()
{
	m_Attack = false;
	m_End = true;
	m_ChaseTime = 0.f;

	// ��ƼŬ ��Ȱ��ȭ
	if (m_RollParticle != nullptr)
	{
		m_RollParticle->Transform()->SetRelativePos(Vec3(0, 30, 0));
	}
}

void CBugRollingScript::Dead()
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_84_MonsterBugRolling_Death.wav");
	pSound->Play(1, 1.f, false);

	// ���� ���� �ı� ����Ʈ ���
	CGameObject* deadEffect = new CGameObject;
	deadEffect->AddComponent(new CTransform);
	deadEffect->AddComponent(new CMeshRender);
	deadEffect->AddComponent(new CFlipBookComponent);
	deadEffect->AddComponent(new CAniFinishDestroyScript);

	deadEffect->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	deadEffect->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 1));

	deadEffect->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	deadEffect->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Ptr<CFlipBook> pFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Effect\\monsterEffect.flip");
	deadEffect->FlipBookComponent()->AddFlipBook(0, pFlip);
	deadEffect->FlipBookComponent()->Play(0, 10, false);

	CreateObject(deadEffect, 0);

	DeleteObject(GetOwner());
}

void CBugRollingScript::HitEffect()
{
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_83_MonsterBugRolling_Hit.wav");
	pSound->Play(1, 1.f, false);
}

