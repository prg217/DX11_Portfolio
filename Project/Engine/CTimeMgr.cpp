#include "pch.h"
#include "CTimeMgr.h"

#include "CKeyMgr.h"

#include "CEngine.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CFontMgr.h"

CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{}
	, m_llFrequency{}
	, m_FPS(0)
	, m_DeltaTime(0.f)
	, m_Time(0.f)
	, m_E_DeltaTime(0.f)
	, m_E_Time(0.f)
	, m_TimeInfo{}
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	// �ʴ� 1000 �� ī�����ϴ� GetTickCount �Լ��� �̼��� �ð��� �����ϱ⿡�� ��Ȯ���� ��������.

	// 1�ʿ� �����ִ� ī��Ʈ ������ ��´�.
	QueryPerformanceFrequency(&m_llFrequency);

	QueryPerformanceCounter(&m_llCurCount);
	m_llPrevCount = m_llCurCount;	
}

void CTimeMgr::Tick()
{
	// ���� ī��Ʈ ���
	QueryPerformanceCounter(&m_llCurCount);

	// ���� ī��Ʈ�� ���� ī��Ʈ�� ���̰��� ���ؼ� 1������ ���� �ð����� ���
	m_E_DeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// DT ����
	if (1.f / 60.f < m_E_DeltaTime)
		m_E_DeltaTime = 1.f / 60.f;

	// �����ð��� ���ؼ� ���α׷��� ����� ���ķ� ������ �ð����� ���
	m_E_Time += m_E_DeltaTime;

	// ���� ī��Ʈ ���� ���� ī��Ʈ�� �����ص�
	m_llPrevCount = m_llCurCount;

	// �ʴ� ���� Ƚ��(FPS) ���
	++m_FPS;

	// 1�ʿ� �ѹ��� TextOut ���
	static float AccTime = 0.f;
	AccTime += m_E_DeltaTime;
	
	if (1.f < AccTime)
	{		
		swprintf_s(m_TimeInfo, L"DeltaTime : %f, FPS : %d ", m_E_DeltaTime, m_FPS);
		//TextOut(CEngine::GetInst()->GetMainDC(), 10, 10, szBuff, wcslen(szBuff));		
		AccTime = 0.f;
		m_FPS = 0;
	}

	// Level �� DT
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (nullptr == pCurLevel || pCurLevel->GetState() != LEVEL_STATE::PLAY)
	{
		m_DeltaTime = 0.f;	
	}
	else
	{
		m_DeltaTime = m_E_DeltaTime;
	}

	m_Time += m_DeltaTime;


	// GlobalData
	g_GlobalData.g_DT = m_DeltaTime;
	g_GlobalData.g_EngineDT = m_E_DeltaTime;
	g_GlobalData.g_Time = m_Time;
	g_GlobalData.g_EngineTime = m_E_Time;

	if (KEY_PRESSED(KEY::RBTN))
	{
		g_GlobalData.g_EngineDT = 0.f;
	}
}

void CTimeMgr::Render()
{
	CFontMgr::GetInst()->DrawFont(m_TimeInfo, 10, 20, 16, FONT_RGBA(255, 20, 20, 255));
}
