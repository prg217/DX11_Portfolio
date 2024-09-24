#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CTaskMgr.h"
#include "CPathMgr.h"

LEVEL_LOAD  CLevelMgr::g_LavelLoadFunc = nullptr;

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_CurLevel)
		delete m_CurLevel;
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (m_CurLevel)
	{
		return m_CurLevel->FindObjectByName(_Name);
	}

	return nullptr;
}

void CLevelMgr::LevelChanged()
{
	CTaskMgr::GetInst()->AddTask(tTask{TASK_TYPE::LEVEL_CHANGED});
}

void CLevelMgr::SetLevelLoad(LEVEL_LOAD _Func)
{
	g_LavelLoadFunc = _Func;
}

void CLevelMgr::LevelNameChange(wstring _LeveName)
{
	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"level\\";
	strInitPath += _LeveName;
	strInitPath += L".lv";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	CLevel* pLoadedLevel = g_LavelLoadFunc(strInitPath);
	fclose(File);
	ChangeLevel(pLoadedLevel, LEVEL_STATE::PLAY);
}


void CLevelMgr::Init()
{	
}

void CLevelMgr::Progress()
{
	m_LevelChanged = false;
	if (nullptr == m_CurLevel)
		return;

	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}

	m_CurLevel->ClearObject();
	m_CurLevel->FinalTick();
}

bool CLevelMgr::ChangeStopLevel(CLevel* _NextLevel)
{
	if (m_CurLevel == _NextLevel)
		return false;

	if (nullptr != m_CurLevel)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	// 레벨이 변경된 이벤트 ON
	LevelChanged();

	return true;
}