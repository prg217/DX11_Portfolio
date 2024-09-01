#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

// Vertex Shader
struct VTX_IN
{
    float3 vPos     : POSITION;
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor    : COLOR;
    float2 vUV       : TEXCOORD;
    
    float3 vWorldPos : POSITION;
};

VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
  
    // LocalSpace -> WorldSpace
    // float3 x float4x4(matrix)
    // float3 를 float4 로 차수를 맞추어준다.
    // 동차좌표를 1 로 설정, 상태행렬 4행에 들어있는 이동을 적용받겠다는 뜻
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;    
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;
    
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{        
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
        
    // FlipBook 을 사용한다.
    if (UseFlipbook)
    {
        // _in.vUV : 스프라이를 참조할 위치를 비율로 환산한 값                
        float2 BackGroundLeftTop = LeftTopUV - (BackGroundUV - SliceUV) / 2.f;
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
        vSpriteUV -= OffsetUV;
        
        bool isPixelOutline = false;

        // 현재 픽셀 색상과 알파 값 샘플링
        float4 color = g_AtlasTex.Sample(g_sam_1, vSpriteUV);
        
        if (UseOutline == 1)
        {
            // 텍셀 크기 계산 (텍스처의 크기에 따라 조정 필요)
            float2 texelSize = float2(1.0 / TexSize.x, 1.0 / TexSize.y);
            // 주변 8개 픽셀의 UV 좌표 계산
            float2 offsets[8] =
            {
                float2(-texelSize.x, 0.0), // Left
                float2(texelSize.x, 0.0), // Right
                float2(0.0, -texelSize.y), // Up
                float2(0.0, texelSize.y), // Down
                float2(-texelSize.x, -texelSize.y), // UpLeft
                float2(texelSize.x, -texelSize.y), // UpRight
                float2(-texelSize.x, texelSize.y), // DownLeft
                float2(texelSize.x, texelSize.y) // DownRight
            };

            // 텍셀 크기와 외곽선 두께를 설정
            float outlineThickness = 3.5; // 외곽선 두께
            float threshold = 0.1f;
            
            if (color.a <= threshold)
            {
                // 주변 픽셀을 체크하여 외곽선 여부 결정
                for (int i = 0; i < 8; i++)
                {
                    float2 offset = offsets[i] * outlineThickness;
                    float2 nearbyUV = vSpriteUV + offset;
                    float4 nearbyColor = g_AtlasTex.Sample(g_sam_1, nearbyUV);

                    // 주변 픽셀의 알파 값이 임계값 이상인 경우, 외곽선으로 설정
                    if (nearbyColor.a > threshold)
                    {
                        // 스프라이트 범위 내부에 있는 알파값 이상만 해당
                        if (LeftTopUV.x <= nearbyUV.x && nearbyUV.x <= LeftTopUV.x + SliceUV.x
                        && LeftTopUV.y <= nearbyUV.y && nearbyUV.y <= LeftTopUV.y + SliceUV.y)
                        {
                            isPixelOutline = true;
                            break;
                        }
                    }
                }

                // 현재 픽셀이 외곽선으로 설정될 조건
                if (isPixelOutline)
                {
                // 현재 픽셀을 외곽선 색상으로 설정
                    vColor = float4(1.0, 1.0, 1.0, 1.0);
                }
            }
        }

        // 현재 픽셀이 스프라이트 영역 안에 있는지 확인
        bool isWithinSprite = (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x
                  && LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y);

        if (isWithinSprite && !isPixelOutline)
        {
            vColor = color;
        }
        // 외곽선 아닌 곳은 투명하게
        else if (!isPixelOutline)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
    }
    
    // FlipBook 을 사용하지 않는다.
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        else
        {
            //vColor = float4(1.f, 0.f, 1.f, 1.f);
            discard;
        }
    }
    
    if(vColor.a == 0.f)
    {    
        discard;
    }
    
    // 광원 적용      
    tLight Light = (tLight) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }
    
    vColor.rgb = vColor.rgb * Light.Color.rgb 
               + vColor.rgb * Light.Ambient.rgb;
    
    return vColor;
}



float4 PS_Std2D_Alphablend(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
        
    // FlipBook 을 사용한다.
    if (UseFlipbook)
    {
        // _in.vUV : 스프라이를 참조할 위치를 비율로 환산한 값                
        float2 BackGroundLeftTop = LeftTopUV - (BackGroundUV - SliceUV) / 2.f;
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
        vSpriteUV -= OffsetUV;
                
        if (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x
            && LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y)
        {
            vColor = g_AtlasTex.Sample(g_sam_1, vSpriteUV);
        }
        else
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
    }
    
    // FlipBook 을 사용하지 않는다.
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        else
        {
            //vColor = float4(1.f, 0.f, 1.f, 1.f);
            discard;
        }
    }
    
    if (vColor.a == 0.f)
    {
        discard;
    }
    
    // 광원 적용      
    tLight Light = (tLight) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }
    
    vColor.rgb = vColor.rgb * Light.Color.rgb 
               + vColor.rgb * Light.Ambient.rgb;
    
    return vColor;
}

#endif