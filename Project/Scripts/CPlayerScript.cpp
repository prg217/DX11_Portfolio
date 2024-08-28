#include "pch.h"
#include "CPlayerScript.h"

#include <Engine/enum.h>

#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(400.f)
	, m_IsRun(false)
	, m_MoveCount(0)
	, m_SaveTime(0)
	, m_AllowedTime(0.1f)
	, m_CurMS(MovementState::IDLE)
{	
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
	AddScriptParam(SCRIPT_PARAM::TEXTURE, "Test", &m_Texture);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Missile", &m_MissilePref);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();
	//m_MissilePref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MissilePref");


}

void CPlayerScript::Tick()
{
	Move();

	if (KEY_PRESSED(KEY::Q))
		//FlipBookComponent()->Play(1, 10, true);

	if (KEY_PRESSED(KEY::Z))
	{
		//MeshRender()->GetMaterial()->SetScalarParam(INT_0, 1);
		Vec3 vRot = Transform()->GetRelativeRoatation();
		vRot.z += DT * XM_PI;
		Transform()->SetRelativeRotation(vRot);

	/*	Vec3 vScale = Transform()->GetRelativeScale();
		vScale += DT * 100.f * Vec3(1.f, 1.f, 1.f);
		Transform()->SetRelativeScale(vScale);*/
	}
	else
	{
		//MeshRender()->GetMaterial()->SetScalarParam(INT_0, 0);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		// 미사일 발사
		if (nullptr != m_MissilePref)
		{
			Instantiate(m_MissilePref, 5, Transform()->GetWorldPos(), L"Missile");
		}		
	}	


}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	DeleteObject(_OtherObject);

	Vec3 vScale = Transform()->GetRelativeScale();

	vScale += Vec3(10.f, 10.f, 0.f);
	Collider2D()->SetScale(Collider2D()->GetScale() + Vec3(10.f, 10.f, 0.f));

	Transform()->SetRelativeScale(vScale);
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
	SaveAssetRef(m_Texture, _File);
	SaveAssetRef(m_MissilePref, _File);	
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
	LoadAssetRef(m_Texture, _File);
	LoadAssetRef(m_MissilePref, _File);
}

void CPlayerScript::Move()
{
	Vec3 vPos = Transform()->GetRelativePos();

	// Run
	if (KEY_TAP(KEY::LSHIFT))
	{
		m_IsRun = true;
		m_Speed = 550.f;
	}
	if (KEY_RELEASED(KEY::LSHIFT))
	{
		m_IsRun = false;
		m_Speed = 400.f;
	}

	// Animation
	switch (m_CurMS)
	{
	case MovementState::IDLE:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE, 10, true);
		break;
	case MovementState::IDLE_BACK:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_BACK, 10, true);
		break;
	case MovementState::IDLE_LEFT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_LEFT, 10, true);
		break;
	case MovementState::IDLE_RIGHT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_RIGHT, 10, true);
		break;
	case MovementState::IDLE_LEFTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_LEFTDOWN, 10, true);
		break;
	case MovementState::IDLE_LEFTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_LEFTUP, 10, true);
		break;
	case MovementState::IDLE_RIGHTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_RIGHTDOWN, 10, true);
		break;
	case MovementState::IDLE_RIGHTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_IDLE_RIGHTUP, 10, true);
		break;
	case MovementState::WALK_DOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_DOWN, 10, true);
		break;
	case MovementState::WALK_UP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_UP, 10, true);
		break;
	case MovementState::WALK_LEFT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_LEFT, 10, true);
		break;
	case MovementState::WALK_RIGHT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_RIGHT, 10, true);
		break;
	case MovementState::WALK_LEFTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_LEFTDOWN, 10, true);
		break;
	case MovementState::WALK_LEFTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_LEFTUP, 10, true);
		break;
	case MovementState::WALK_RIGHTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_RIGHTDOWN, 10, true);
		break;
	case MovementState::WALK_RIGHTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_WALK_RIGHTUP, 10, true);
		break;
	case MovementState::RUN_DOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_DOWN, 10, true);
		break;
	case MovementState::RUN_UP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_UP, 10, true);
		break;
	case MovementState::RUN_LEFT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_LEFT, 10, true);
		break;
	case MovementState::RUN_RIGHT:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_RIGHT, 10, true);
		break;
	case MovementState::RUN_LEFTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_LEFTDOWN, 10, true);
		break;
	case MovementState::RUN_LEFTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_LEFTUP, 10, true);
		break;
	case MovementState::RUN_RIGHTDOWN:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_RIGHTDOWN, 10, true);
		break;
	case MovementState::RUN_RIGHTUP:
		FlipBookComponent()->Play((int)FLIPBOOK_IDX::OGU_RUN_RIGHTUP, 10, true);
		break;
	default:
		break;
	}

	if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::UP))
	{
		if (m_IsRun)
		{
			m_CurMS = MovementState::RUN_LEFTUP;
		}
		else
		{
			m_CurMS = MovementState::WALK_LEFTUP;
		}

		m_SaveTime = TIME;
	}
	else if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::DOWN))
	{
		if (m_IsRun)
		{
			m_CurMS = MovementState::RUN_LEFTDOWN;
		}
		else
		{
			m_CurMS = MovementState::WALK_LEFTDOWN;
		}

		m_SaveTime = TIME;
	}
	else if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::UP))
	{
		if (m_IsRun)
		{
			m_CurMS = MovementState::RUN_RIGHTUP;
		}
		else
		{
			m_CurMS = MovementState::WALK_RIGHTUP;
		}

		m_SaveTime = TIME;
	}
	else if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::DOWN))
	{
		if (m_IsRun)
		{
			m_CurMS = MovementState::RUN_RIGHTDOWN;
		}
		else
		{
			m_CurMS = MovementState::WALK_RIGHTDOWN;
		}

		m_SaveTime = TIME;
	}
	else if (TIME - m_SaveTime > m_AllowedTime)
	{
		if (KEY_PRESSED(KEY::LEFT))
		{
			if (m_IsRun)
			{
				m_CurMS = MovementState::RUN_LEFT;
			}
			else
			{
				m_CurMS = MovementState::WALK_LEFT;
			}
		}
		else if (KEY_PRESSED(KEY::RIGHT))
		{
			if (m_IsRun)
			{
				m_CurMS = MovementState::RUN_RIGHT;
			}
			else
			{
				m_CurMS = MovementState::WALK_RIGHT;
			}
		}
		else if (KEY_PRESSED(KEY::UP))
		{
			if (m_IsRun)
			{
				m_CurMS = MovementState::RUN_UP;
			}
			else
			{
				m_CurMS = MovementState::WALK_UP;
			}
		}
		else if (KEY_PRESSED(KEY::DOWN))
		{
			if (m_IsRun)
			{
				m_CurMS = MovementState::RUN_DOWN;
			}
			else
			{
				m_CurMS = MovementState::WALK_DOWN;
			}
		}

	}

	if (KEY_RELEASED(KEY::LEFT))
	{
		if (m_CurMS == MovementState::WALK_LEFTUP || m_CurMS == MovementState::RUN_LEFTUP)
		{
			if (TIME - m_SaveTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = MovementState::IDLE_LEFTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = MovementState::IDLE_LEFT;
			}

		}
		else if (m_CurMS == MovementState::WALK_LEFTDOWN || m_CurMS == MovementState::RUN_LEFTDOWN)
		{
			if (TIME - m_SaveTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = MovementState::IDLE_LEFTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = MovementState::IDLE_LEFT;
			}
		}
		else if (m_CurMS == MovementState::WALK_LEFT || m_CurMS == MovementState::RUN_LEFT)
		{
			m_CurMS = MovementState::IDLE_LEFT;
		}
	}

	if (KEY_RELEASED(KEY::RIGHT))
	{
		if (m_CurMS == MovementState::WALK_RIGHTUP || m_CurMS == MovementState::RUN_RIGHTUP)
		{
			if (TIME - m_SaveTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = MovementState::IDLE_RIGHTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = MovementState::IDLE_RIGHT;
			}
		}
		else if (m_CurMS == MovementState::WALK_RIGHTDOWN || m_CurMS == MovementState::RUN_RIGHTDOWN)
		{
			if (TIME - m_SaveTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = MovementState::IDLE_RIGHTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = MovementState::IDLE_RIGHT;
			}
		}
		else if (m_CurMS == MovementState::WALK_RIGHT || m_CurMS == MovementState::RUN_RIGHT)
		{
			m_CurMS = MovementState::IDLE_RIGHT;
		}
	}

	if (KEY_RELEASED(KEY::UP))
	{
		if (m_CurMS == MovementState::WALK_LEFTUP || m_CurMS == MovementState::RUN_LEFTUP)
		{
			if (TIME - m_SaveTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = MovementState::IDLE_LEFTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = MovementState::IDLE_BACK;
			}
		}
		else if (m_CurMS == MovementState::WALK_RIGHTUP || m_CurMS == MovementState::RUN_RIGHTUP)
		{
			if (TIME - m_SaveTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = MovementState::IDLE_RIGHTUP;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = MovementState::IDLE_RIGHT;
			}
		}
		else if (m_CurMS == MovementState::WALK_UP || m_CurMS == MovementState::RUN_UP)
		{
			m_CurMS = MovementState::IDLE_BACK;
		}
	}

	if (KEY_RELEASED(KEY::DOWN))
	{
		if (m_CurMS == MovementState::WALK_LEFTDOWN || m_CurMS == MovementState::RUN_LEFTDOWN)
		{
			if (TIME - m_SaveTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = MovementState::IDLE_LEFTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = MovementState::IDLE;
			}
		}
		else if (m_CurMS == MovementState::WALK_RIGHTDOWN || m_CurMS == MovementState::RUN_RIGHTDOWN)
		{
			if (TIME - m_SaveTime <= m_AllowedTime)
			{
				m_MoveCount++;

				if (m_MoveCount >= 2)
				{
					m_CurMS = MovementState::IDLE_RIGHTDOWN;
				}
			}
			else
			{
				m_MoveCount = 0;

				m_CurMS = MovementState::IDLE;
			}
		}
		else if (m_CurMS == MovementState::WALK_DOWN || m_CurMS == MovementState::RUN_DOWN)
		{
			m_CurMS = MovementState::IDLE;
		}
	}

	// Move
	if (KEY_PRESSED(KEY::LEFT))
	{
		vPos.x -= DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::RIGHT))
	{
		vPos.x += DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::UP))
	{
		vPos.y += DT * m_Speed;
	}
	if (KEY_PRESSED(KEY::DOWN))
	{
		vPos.y -= DT * m_Speed;
	}

	Transform()->SetRelativePos(vPos);
}
