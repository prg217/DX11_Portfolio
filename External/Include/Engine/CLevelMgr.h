#pragma once

class CLevel;
class CGameObject;

typedef CLevel* (*LEVEL_LOAD)(const wstring&);

class CLevelMgr
	: public CSingleton<CLevelMgr>
{
	SINGLE(CLevelMgr);
private:
	static LEVEL_LOAD  g_LavelLoadFunc;

	CLevel*		m_CurLevel;
	bool		m_LevelChanged;

public:
	CLevel* GetCurrentLevel() { return m_CurLevel; }
	CGameObject* FindObjectByName(const wstring& _Name);

	void LevelChanged();
	bool IsLevelChanged() { return m_LevelChanged; }

	void SetLevelLoad(LEVEL_LOAD _Func);
	void LevelNameChange(wstring _LeveName);

private:
	bool ChangeStopLevel(CLevel* _NextLevel);

public:
	void Init();
	void Progress();

	friend class CTaskMgr;
	friend class CEditorMgr;
};

