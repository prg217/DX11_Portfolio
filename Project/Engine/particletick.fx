#ifndef _PARTICLE_TICK
#define _PARTICLE_TICK

#include "value.fx"
#include "struct.fx"


RWStructuredBuffer<tParticle> ParticleBuffer : register(u0);

#define MAX_COUNT       g_int_0
#define Particle        ParticleBuffer[_ID.x]

[numthreads(1024, 1, 1)]
void CS_ParticleTick(int3 _ID : SV_DispatchThreadID)
{
    if (MAX_COUNT <= _ID.x)
        return;
    
    if (false == Particle.Active)
        return;
    
    
    Particle.vWorldPos += Particle.vVelocity * g_EngineDT;
}

// 1. ��ƼŬ ������, ��Ȱ��ȭ ��ƼŬ ó��
//  

// 2. ��ƼŬ ������ ����ȭ
// 3. ��ƼŬ CS ���� ����.


#endif