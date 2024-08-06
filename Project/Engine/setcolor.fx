#ifndef _SETCOLOR
#define _SETCOLOR

#include "value.fx"

RWTexture2D<float4> g_TargetTex : register(u0);
#define COLOR g_vec4_0

// SV_GroupID : �����尡 ���� �׷��� ���̵� �޴´�.
// SV_GroupThreadID : ���� �׷쿡�� �������� ���̵�
// SV_GroupIndex : SV_GroupThreadID �� 1�������� ��ȯ
// SV_DispatchThreadID : ��ü���� ID

// �׷�� ������ ��
[numthreads(32, 32, 1)]
void CS_SetColor(int3 _ID : SV_DispatchThreadID)
{
    // �ȼ��� �ʰ��ؼ� �����ϴ� ������(�ʰ� ������ ������)
    if(g_int_0 <= _ID.x || g_int_1 <= _ID.y )
        return;
    
    g_TargetTex[_ID.xy] = COLOR;
}

#endif