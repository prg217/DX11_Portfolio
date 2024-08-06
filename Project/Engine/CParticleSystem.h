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
    virtual void SaveToFile(FILE* _File) override;   // 파일에 컴포넌트의 정보를 저장
    virtual void LoadFromFile(FILE* _File) override; // 파일에 컴포넌트의 정보를 로드

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    ~CParticleSystem();
};

