#ifndef _PARTICLE
#define _PARTICLE

#include "value.fx"
#include "struct.fx"

StructuredBuffer<tParticle> ParticleBuffer : register(t20);


struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    uint InstID : SV_InstanceID;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    uint InstID : FOG;
};


VS_OUT VS_Particle(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // ũ�� -> �̵�
    float3 vWorldPos = _in.vPos * float3(100.f, 100.f, 1.f) + ParticleBuffer[_in.InstID].vWorldPos;
        
    output.vPosition = mul(mul(float4(vWorldPos, 1.f), matView), matProj);
    output.vUV = _in.vUV;    
    output.InstID = _in.InstID;
    
    return output;    
}


// 1. ��Ȱ��ȭ ��ƼŬ ó��
// GeometryShader ���� ������ ���� �� ����
// ��Ȱ��ȭ�� ��ƼŬ�� �������� ���ʸ�, ��½�Ʈ���� �����, �߰��� ���������� �����Ŵ

// 2. ��ƼŬ ������ ó��






float4 PS_Particle(VS_OUT _in) : SV_Target
{       
    float4 vParticleColor = float4(1.f, 0.f, 0.f, 1.f);
   
    if (g_btex_0)
    {
        vParticleColor = g_tex_0.Sample(g_sam_0, _in.vUV);
        vParticleColor *= ParticleBuffer[_in.InstID].vColor;
    }
        
    return vParticleColor;
}




#endif