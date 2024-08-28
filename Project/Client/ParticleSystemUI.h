#pragma once
#include "ComponentUI.h"

class CParticleSystem;

class ParticleSystemUI :
    public ComponentUI
{
public:
    virtual void Update() override;

private:
    CParticleSystem*    m_Particle;
    tParticleModule     m_ParticleModule;

    Ptr<CTexture>       m_ParticleTex;
    float               m_WidthSize;
    float               m_Ratio;        // ���� �ػ� ��� ��� �̹����� ����

    void SaveLoad();
    void SelectTexture(DWORD_PTR _ListUI);

public:
    void SetParticleTex(Ptr<CTexture> _Tex);

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

