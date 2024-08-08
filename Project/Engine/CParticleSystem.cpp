#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)	
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_Time(0.f)
	, m_MaxParticeCount(100)
{
	// Mesh / Material 
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"));

	// ParticleTick ComputeShader
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();

	m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\particle\\TX_GlowScene_2.png");

	// ��ƼŬ 100�� �ʱ� ����
	tParticle arrParticle[100] = {};

	float Angle = XM_2PI / m_MaxParticeCount;

	for (int i = 0; i < m_MaxParticeCount; ++i)
	{
		arrParticle[i].Active = false;
		arrParticle[i].Mass = 1.f;
		arrParticle[i].vLocalPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldScale = Vec3(20.f, 20.f, 0.f);
		arrParticle[i].vColor = Vec4(0.9f, 0.34f, 0.5f, 1.f);

		arrParticle[i].vVelocity = Vec3(cosf(Angle * (float)i), sinf(Angle * (float)i), 0.f) * 5.f;
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticeCount, SB_TYPE::SRV_UAV, true, arrParticle);

	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true, nullptr);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
}

void CParticleSystem::FinalTick()
{
	// SpawnCount
	m_Time += EngineDT;
	if (1.f <= m_Time)
	{
		tSpawnCount count = {};
		count.iSpawnCount = 1;
		m_SpawnCountBuffer->SetData(&count);
		m_Time = 0.f;
	}

	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCount(m_SpawnCountBuffer);
	m_TickCS->Execute();
}

void CParticleSystem::Render()
{
	// ��ġ���� ���ε�
	Transform()->Binding();

	// ��ƼŬ ���� ���ε�
	m_ParticleBuffer->Binding(20); // t20

	// �������� ���ε�
	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->Binding();
	

	// ������
	GetMesh()->Render_Particle(m_MaxParticeCount);

	// ��ƼŬ ���� ���ε� ����
	m_ParticleBuffer->Clear(20);
}

void CParticleSystem::SaveToFile(FILE* _File)
{

}

void CParticleSystem::LoadFromFile(FILE* _File)
{

}