#ifndef _POSTPROCESS
#define POSTPROCESS

#include "value.fx"

// ==========================
// GrayFilterShader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : TargetCopyTex
// g_tex_1  : NoiseTexture 1
// g_tex_2  : NoiseTexture 2
// g_tex_3  : NoiseTexture 3
// ===========================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // Proj 행렬을 곱한 결과는 각 xyz 에 자신의 ViewZ 가 곱혀져있는 형태이다.
    // W 자리에 자신의 ViewZ 가 출력되기 때문에 이것으로 각 xyz 를 나누어야 실제 Proj 좌표가 나온다.
    // 따라서 Rasterizer State 에 투영행렬을 곱한 결과를 전달하면 각 xyz 를 w 로 나누어서 사용한다.    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

// 흔들거림 효과
// Function to generate a random value between -1 and 1 (Remember this is important!!!!)
float random(in float2 vUV, in float seed)
{
    return frac(sin(dot(vUV, float2(12.9898, 78.233)) + seed) * 43758.5453);
}
float4 PPSwing(in float2 vUV)
{
    const float Frequency = 0.2;
    const float Amplitude = 0.1;
    const float Trippy = 0.5;

    // Time
    float time = g_Time;

    // Distortion thing (IMPORTANT!)
    float seed = floor(time / 5.0);
    float randomDistortion1 = random(vUV, seed);
    float randomDistortion2 = random(vUV, seed + 1.0);
    float randomDistortion3 = random(vUV, seed + 2.0);

    // Distortion calculations
    float2 distortion = float2(
        sin((vUV.y + randomDistortion1) * Frequency + time) * Amplitude,
        sin((vUV.x + randomDistortion2) * Frequency + time) * Amplitude
    );

    distortion += float2(
        sin((vUV.x + vUV.y + randomDistortion3) * Frequency + time) * Amplitude * 0.5,
        sin((vUV.x - vUV.y - randomDistortion1) * Frequency + time) * Amplitude * 0.5
    );

    distortion += float2(
        sin(time) * Trippy,
        cos(time) * Trippy
    );

    // Apply distortion
    float2 distortedUV = vUV + distortion;

    // Sample the texture
    float4 texColor = g_tex_0.Sample(g_sam_0, distortedUV);

    // Output
    return texColor;
}

// 모래먼지
float4 PPDust(in float2 vUV)
{
    float distortAmount = 0.0015;
    float distortZoom = 1.2;
    float distortSpeed = 0.1;
    float lerpIntensity = 0.6;
    
    float3 mainColor = float3(0.5, 0.45, 0.4) * 0.8;
    
    // 노이즈 이미지
    float distortNoise = g_tex_1.Sample(g_sam_0, float2(frac(vUV.x * distortZoom - g_Time * distortSpeed), vUV.y * distortZoom).r);
    
    distortNoise = smoothstep(0.11, 0.45, distortNoise);

    float2 distortUV = (float2(distortNoise * 1., distortNoise * 1.) * distortAmount);
    
    float4 color = g_tex_0.Sample(g_sam_0, vUV + distortUV);
    
    // 모래?같은 이미지 추가 필요
    float sandNoise = g_tex_2.Sample(g_sam_0, float2(frac(vUV.x - g_Time * 0.8), frac(vUV.y + g_Time * 0.055)) + distortUV).r;
    float sandNoise2 = g_tex_2.Sample(g_sam_0, float2(frac(vUV.x * 1.2 - g_Time * 0.4), frac(vUV.y * 1.2 + g_Time * 0.06)) + distortUV).r;
    float sandNoise3 = g_tex_2.Sample(g_sam_0, float2(frac(vUV.x * 0.8 - g_Time * 0.6), frac(vUV.y * 0.8 + g_Time * 0.035)) + distortUV).r;

    float finalSandNoise = sandNoise * 0.233 + sandNoise2 * 0.433 + sandNoise3 * 0.333;
    float sineLerpModifier = (1. + sin(3.1415 * g_Time * 0.4)) * 0.5;
    color = lerp(color, float4(mainColor, 1.), lerpIntensity * sineLerpModifier);
    color += float4(mainColor, 1.) * lerp(0.2, 1.0, finalSandNoise * lerp(0.5, 0.9, sineLerpModifier));

    return color;
}

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{
    // GrayFilter
    //float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);    
    //float Average = (vColor.x + vColor.y + vColor.z) / 3.f;
    //vColor = float4(Average, Average, Average, 1.f); 
        
    // Cos Distortion
    //float2 vUV = _in.vUV;    
    //vUV.y += cos( (vUV.x + g_EngineTime * 0.1f) * PI * 12.f) * 0.01;    
    //float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
    
    // Noise
    //float2 vUV = _in.vUV;
    //vUV.x += g_EngineTime * 0.1;    
    //float4 vNoise = g_tex_3.Sample(g_sam_0, vUV);
    //vNoise = (vNoise * 2.f - 1.f) * 0.01f;    
    //vUV = _in.vUV + vNoise.xy;
    //float4 vColor = g_tex_0.Sample(g_sam_0, vUV);    
    //vColor.b *= 1.5f;   
    
    float2 vUV = _in.vUV;
    float2 vPos = _in.vPosition;
    // 흔들거림 효과
    float4 vColor = PPSwing(vUV);
    // 모래먼지 효과 (알맞은 노이즈 이미지 추가 필요)
    //float4 vColor = PPDust(vUV);
    
    return vColor;

}

// ==========================
// Distortion Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : TargetCopyTex
// g_tex_1  : NoiseTexture 1
// g_tex_2  : NoiseTexture 2
// g_tex_3  : NoiseTexture 3
// ===========================
VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

// 볼록 효과
float2 barrelDistortion(in float2 vUV, in float strength)
{
    float2 center = float2(0.5f, 0.5f);
    float2 d = vUV - center;
    float r2 = d.x * d.x + d.y * d.y;
    float rr = 0.5f * 0.5f;
    float k1 = 0.1f;
    float k2 = 0.2f;
    float k3 = -0.3f;
    float s = pow(1.0f + k1 * r2 + k2 * r2 * r2 + k3 * r2 * r2 * r2, strength);
    float sr = pow(1.0f + k1 * rr + k2 * rr * rr + k3 * rr * rr * rr, strength);
    return d * s / sr + center;
}
float sdBox(in float2 p, in float2 b)
{
    float2 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}
float sdRoundedBox(in float2 p, in float4 b, in float r)
{
    float2 q = abs(p - b.xy) - b.zw + r;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
}
float4 PPConvex(in float2 vUV)
{
    float4 color = float4(0.f, 0.f, 0.f, 0.f);
    float distortStrength = cos(g_Time * 0.5f);

    float2 distortedUV = barrelDistortion(vUV, distortStrength * 10.0f);
    float4 mainTex = g_tex_0.Sample(g_sam_0, distortedUV);
    
    float2 prop = float2(1.0f, g_Resolution.y / g_Resolution.x);
    float fw = length(fwidth(distortedUV));
    float sd = sdBox(distortedUV * prop - 0.5f * prop, 0.5f * prop);
    mainTex.a = smoothstep(fw, 0.0f, sd);
        
    color.rgb = lerp(color.rgb, mainTex.rgb, mainTex.a);
    color.a = color.a * (1.0f - mainTex.a) + mainTex.a;
    return color;
}

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    // 1. 렌더타겟 해상도 정보 (전역 상수버퍼)    
    // 2. 픽셀쉐이더의 픽셀 좌표
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    //float2 vNoiseUV = vScreenUV;
    //vNoiseUV.x += g_EngineTime * 0.1;
    //float4 vNoise = g_tex_3.Sample(g_sam_0, vNoiseUV);
    //
    //vNoise = (vNoise * 2.f - 1.f) * 0.01f;
    //vScreenUV = vScreenUV + vNoise.xy;
    //float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    
    // 볼록 효과
    float4 vColor = PPConvex(vScreenUV);
    
    return vColor;
}

// ==========================
// Blur Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : Target
// ===========================

static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

static float CrossFilter[13] = { 0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
static float Total = 6.2108f;

struct VS_OUT_BLUR
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

VS_OUT_BLUR VS_Blur(VS_IN _in, uint _InstID : SV_InstanceID)
{
    VS_OUT_BLUR output = (VS_OUT_BLUR) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    output.InstID = _InstID;
    
    return output;
}

float4 PS_Blur(VS_OUT_BLUR _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
        
    float2 vUVStep = 1.f / g_Resolution;
    vUVStep *= 3.f;
    
    if (_in.InstID == 0)
    {
        for (int i = 0; i < 13; ++i)
        {
            float2 vUV = _in.vUV + float2(vUVStep.x * (-6 + i), 0.f);
            vColor += g_tex_0.Sample(g_sam_2, vUV) * CrossFilter[i];
        }
    }
    else if (_in.InstID == 1)
    {
        for (int j = 0; j < 13; ++j)
        {
            float2 vUV = _in.vUV + float2(0.f, vUVStep.y * (-6 + j));
            vColor += g_tex_0.Sample(g_sam_2, vUV) * CrossFilter[j];
        }
    }
    vColor /= Total;
    
    return vColor;
}


VS_OUT VS_EffectMerge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_EffectMerge(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 vEffect = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vEffectBlur = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    float4 vBloom = pow(abs(pow(vEffect, 2.2f)) + abs(pow(vEffectBlur, 2.2f)), 1.f / 2.2f);
    
    return vBloom;
}


#endif