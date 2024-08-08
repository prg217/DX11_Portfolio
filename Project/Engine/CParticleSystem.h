#pragma once
#include "CRenderComponent.h"

#include "CParticleTickCS.h"

class CStructuredBuffer;

struct tSpawnCount
{
    UINT    iSpawnCount;
    UINT    padding[3];
};

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleTickCS>    m_TickCS;
    CStructuredBuffer*      m_ParticleBuffer;       // ��� ��ƼŬ ����
    CStructuredBuffer*      m_SpawnCountBuffer;     // ��ƼŬ Ȱ��ȭ ���� ���޿� ����

    Ptr<CTexture>           m_ParticleTex;
    float                   m_Time;                 // �����ð�

    int                     m_MaxParticeCount;





public:
    void SetParticleTexture(Ptr<CTexture> _Texture) { m_ParticleTex = _Texture; }

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;   // ���Ͽ� ������Ʈ�� ������ ����
    virtual void LoadFromFile(FILE* _File) override; // ���Ͽ� ������Ʈ�� ������ �ε�

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    ~CParticleSystem();
};

