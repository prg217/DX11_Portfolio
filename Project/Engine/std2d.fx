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
    // float3 �� float4 �� ������ ���߾��ش�.
    // ������ǥ�� 1 �� ����, ������� 4�࿡ ����ִ� �̵��� ����ްڴٴ� ��
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;    
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;
    
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{        
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
        
    // FlipBook �� ����Ѵ�.
    if (UseFlipbook)
    {
        // _in.vUV : �������̸� ������ ��ġ�� ������ ȯ���� ��                
        float2 BackGroundLeftTop = LeftTopUV - (BackGroundUV - SliceUV) / 2.f;
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
        vSpriteUV -= OffsetUV;
        
        bool isPixelOutline = false;

        // ���� �ȼ� ����� ���� �� ���ø�
        float4 color = g_AtlasTex.Sample(g_sam_1, vSpriteUV);
        
        if (UseOutline == 1)
        {
            // �ؼ� ũ�� ��� (�ؽ�ó�� ũ�⿡ ���� ���� �ʿ�)
            float2 texelSize = float2(1.0 / TexSize.x, 1.0 / TexSize.y);
            // �ֺ� 8�� �ȼ��� UV ��ǥ ���
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

            // �ؼ� ũ��� �ܰ��� �β��� ����
            float outlineThickness = 3.5; // �ܰ��� �β�
            float threshold = 0.1f;
            
            if (color.a <= threshold)
            {
                // �ֺ� �ȼ��� üũ�Ͽ� �ܰ��� ���� ����
                for (int i = 0; i < 8; i++)
                {
                    float2 offset = offsets[i] * outlineThickness;
                    float2 nearbyUV = vSpriteUV + offset;
                    float4 nearbyColor = g_AtlasTex.Sample(g_sam_1, nearbyUV);

                    // �ֺ� �ȼ��� ���� ���� �Ӱ谪 �̻��� ���, �ܰ������� ����
                    if (nearbyColor.a > threshold)
                    {
                        // ��������Ʈ ���� ���ο� �ִ� ���İ� �̻� �ش�
                        if (LeftTopUV.x <= nearbyUV.x && nearbyUV.x <= LeftTopUV.x + SliceUV.x
                        && LeftTopUV.y <= nearbyUV.y && nearbyUV.y <= LeftTopUV.y + SliceUV.y)
                        {
                            isPixelOutline = true;
                            break;
                        }
                    }
                }

                // ���� �ȼ��� �ܰ������� ������ ����
                if (isPixelOutline)
                {
                // ���� �ȼ��� �ܰ��� �������� ����
                    vColor = float4(1.0, 1.0, 1.0, 1.0);
                }
            }
        }

        // ���� �ȼ��� ��������Ʈ ���� �ȿ� �ִ��� Ȯ��
        bool isWithinSprite = (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x
                  && LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y);

        if (isWithinSprite && !isPixelOutline)
        {
            vColor = color;
        }
        // �ܰ��� �ƴ� ���� �����ϰ�
        else if (!isPixelOutline)
        {
            //vColor = float4(1.f, 1.f, 0.f, 1.f);
            discard;
        }
    }
    
    // FlipBook �� ������� �ʴ´�.
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
    
    // ���� ����      
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
        
    // FlipBook �� ����Ѵ�.
    if (UseFlipbook)
    {
        // _in.vUV : �������̸� ������ ��ġ�� ������ ȯ���� ��                
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
    
    // FlipBook �� ������� �ʴ´�.
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
    
    // ���� ����      
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