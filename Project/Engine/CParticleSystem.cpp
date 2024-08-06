#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)	
	, m_ParticleBuffer(nullptr)
	, m_MaxParticeCount(100)
{
	// Mesh / Material 
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"));

	// 파티클 100개 초기 설정
	tParticle arrParticle[100] = {};
	Vec2 vResolution = CDevice::GetInst()->GetResolution();
	Vec3 vStart = Vec3(-vResolution.x / 2.f, 0.f, 100.f);
	float step = vResolution.x / (float)m_MaxParticeCount;

	for (int i = 0; i < m_MaxParticeCount; ++i)
	{
		arrParticle[i].Active = true;
		arrParticle[i].Mass = 1.f;
		arrParticle[i].vLocalPos = Vec3(0.f, 0.f, 0.f);
		arrParticle[i].vWorldPos = vStart + Vec3(step * (float)i, 0.f, 0.f);
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticeCount, SB_TYPE::SRV_UAV, true, arrParticle);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
}

void CParticleSystem::FinalTick()
{
	
}

void CParticleSystem::Render()
{
	m_ParticleBuffer->Binding(20);

	for (int i = 0; i < m_MaxParticeCount; ++i)
	{
		GetMaterial()->SetScalarParam(INT_0, i);
		GetMaterial()->Binding();
		GetMesh()->Render();
	}
}

void CParticleSystem::SaveToFile(FILE* _File)
{

}

void CParticleSystem::LoadFromFile(FILE* _File)
{

}