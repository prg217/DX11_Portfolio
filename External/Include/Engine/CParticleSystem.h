#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*  m_ParticleBuffer;

    int                 m_MaxParticeCount;




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

