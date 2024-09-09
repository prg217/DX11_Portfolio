#include "pch.h"
#include "CRigidBody.h"

#include "CTimeMgr.h"
#include "CTransform.h"
#include "CCollider2D.h"

CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(1.f)
	, m_InitWalkSpeed(0.f)
	, m_MaxWalkSpeed(0.f)
	, m_MaxGravitySpeed(0.f)
	, m_Friction(500.f)
	, m_GravityAccel(980.f)
	, m_UseGravity(false)
	, m_Ground(false)
	, m_JumpSpeed(400.f)
	//, m_GroundFunc(nullptr)
	//, m_AirFunc(nullptr)
	//, m_GroundInst(nullptr)
	//, m_GroundDelegate(nullptr)
	//, m_AirInst(nullptr)
	//, m_AirDelegate(nullptr)
{

}

CRigidBody::CRigidBody(const CRigidBody& _Origin)
	: CComponent(_Origin)
	, m_Mass(_Origin.m_Mass)
	, m_InitWalkSpeed(_Origin.m_InitWalkSpeed)
	, m_MaxWalkSpeed(_Origin.m_MaxWalkSpeed)
	, m_MaxGravitySpeed(_Origin.m_MaxGravitySpeed)
	, m_Friction(_Origin.m_Friction)
	, m_GravityAccel(_Origin.m_GravityAccel)
	, m_UseGravity(_Origin.m_UseGravity)
	, m_Ground(_Origin.m_Ground)
	, m_JumpSpeed(_Origin.m_JumpSpeed)
	//, m_GroundFunc(nullptr)
	//, m_AirFunc(nullptr)
	//, m_GroundInst(nullptr)
	//, m_GroundDelegate(nullptr)
	//, m_AirInst(nullptr)
	//, m_AirDelegate(nullptr)
{
}

CRigidBody::~CRigidBody()
{

}
/*
void CRigidBody::Jump()
{
	m_VelocityByGravity += Vec2(0.f, -1.f) * m_JumpSpeed;
	SetGround(false);
}
*/
void CRigidBody::FinalTick()
{
	CTransform* pTransform = GetOwner()->Transform();
	assert(pTransform);

	Vec3 vObjPos = pTransform->GetRelativePos();

	// F = M x A, F / M = A
	Vec3 vAccel = m_Force / m_Mass;

	// ���ӵ�
	// �߷��� ����Ҷ�, ���߿��� ���� ����� ���
	if (m_UseGravity && !m_Ground)
	{
		m_Velocity += vAccel * DT * 0.5f;
	}
	else
	{
		m_Velocity = vAccel * DT * 1.f;
	}


	// �ִ� �ӵ� ����
	//if (0.f != m_MaxWalkSpeed && m_MaxWalkSpeed < m_Velocity.Length())
	//{
	//	m_Velocity.Normalize();
	//	m_Velocity *= m_MaxWalkSpeed;
	//}

	// �ּ� �ӵ� ����
	//if (m_InitWalkSpeed != 0.f && m_Velocity.Length() < 10.f && !(vAccel.x == 0.f && vAccel.y == 0.f))
	//{
	//	Vec3 vForceDir = m_Force;
	//	vForceDir.Normalize();
	//	m_Velocity = vForceDir * m_InitWalkSpeed;
	//}

	// ���� RigidBody �� ����� ���� �����鼭, �ӵ��� �ִ°��
	// ������ ���ؼ� ���� �ӵ��� ���δ�
	if ((m_Force.x == 0.f && m_Force.y == 0.f))
	{
		float Speed = m_Velocity.Length();

		// �߷±���� �װ�, ���߻����� ���
		if (m_UseGravity && !m_Ground)
		{
			// ������ �� ���� �����Ѵ�.
			Speed -= m_Friction * DT * 0.2f;
		}
		else
		{
			Speed -= m_Friction * DT;
		}


		// ������ ���ؼ� �ӵ��� ������ ���� ���� ���⶧����, 
		// ���ҵ� �ӷ��� ũ�Ⱑ ������ ���� �ʰ� �����Ѵ�.
		if (Speed < 0)
			Speed = 0.f;

		if (!(m_Velocity.x == 0.f && m_Velocity.y == 0.f))
			m_Velocity.Normalize();

		m_Velocity *= Speed;
	}

	// �߷� ���ӵ��� ���� �ӵ� ����
	if (m_UseGravity && !m_Ground)
	{
		m_VelocityByGravity += Vec3(0.f, 1.f, 0.f) * m_GravityAccel * DT;

		if (0.f != m_MaxGravitySpeed && m_MaxGravitySpeed < m_VelocityByGravity.Length())
		{
			m_VelocityByGravity.Normalize();
			m_VelocityByGravity *= m_MaxGravitySpeed;
		}
	}

	// ���� �ӵ�
	Vec3 vFinalVelocity = m_Velocity;// +m_VelocityByGravity;
	
	// ���� �ӵ��� ���� �̵�
	// �ӵ� = �Ÿ� / �ð�	
	vObjPos -= vFinalVelocity;// *(DT * 400);
	pTransform->SetRelativePos(vObjPos);

	// �̹� ������ �� �ʱ�ȭ
	m_Force = Vec3(0.f, 0.f, 0.f);
	m_AddVelocity = Vec3(0.f, 0.f, 0.f);
}

void CRigidBody::SaveToFile(FILE* _File)
{
	fwrite(&m_Velocity, sizeof(Vec3), 1, _File);
	fwrite(&m_VelocityByGravity, sizeof(Vec3), 1, _File);
	fwrite(&m_AddVelocity, sizeof(Vec3), 1, _File);

	fwrite(&m_Mass, sizeof(float), 1, _File);
	fwrite(&m_Force, sizeof(Vec3), 1, _File);

	fwrite(&m_InitWalkSpeed, sizeof(float), 1, _File);
	fwrite(&m_MaxWalkSpeed, sizeof(float), 1, _File);
	fwrite(&m_MaxGravitySpeed, sizeof(float), 1, _File);

	fwrite(&m_Friction, sizeof(float), 1, _File);

	fwrite(&m_GravityAccel, sizeof(float), 1, _File);

	fwrite(&m_UseGravity, sizeof(bool), 1, _File);
	//bool    m_Ground;               // �� ���� ���ִ��� üũ
	fwrite(&m_JumpSpeed, sizeof(float), 1, _File);
}

void CRigidBody::LoadFromFile(FILE* _File)
{
	fread(&m_Velocity, sizeof(Vec3), 1, _File);
	fread(&m_VelocityByGravity, sizeof(Vec3), 1, _File);
	fread(&m_AddVelocity, sizeof(Vec3), 1, _File);

	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_Force, sizeof(Vec3), 1, _File);

	fread(&m_InitWalkSpeed, sizeof(float), 1, _File);
	fread(&m_MaxWalkSpeed, sizeof(float), 1, _File);
	fread(&m_MaxGravitySpeed, sizeof(float), 1, _File);

	fread(&m_Friction, sizeof(float), 1, _File);

	fread(&m_GravityAccel, sizeof(float), 1, _File);

	fread(&m_UseGravity, sizeof(bool), 1, _File);
	//bool    m_Ground;               // �� ���� ���ִ��� üũ
	fread(&m_JumpSpeed, sizeof(float), 1, _File);
}
