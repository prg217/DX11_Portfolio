#include "pch.h"
#include "CTaskMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CAssetMgr.h"


CTaskMgr::CTaskMgr()
{}

CTaskMgr::~CTaskMgr()
{}

void CTaskMgr::Tick()
{
	ClearGC();

	ExecuteTask();
}

void CTaskMgr::AddTask(const tTask& _Task)
{
	m_vecTask.push_back(_Task);
}

void CTaskMgr::ClearGC()
{
	Delete_Vec(m_GC);
}

void CTaskMgr::ExecuteTask()
{
	for (size_t i = 0; i < m_vecTask.size(); ++i)
	{
		tTask& task = m_vecTask[i];
		switch (task.Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			int LayerIndex = (int)task.Param_0;
			CGameObject* pObject = (CGameObject*)task.Param_1;
			pCurLevel->AddObject(LayerIndex, pObject);

			if (pCurLevel->GetState() != STOP)
			{
				pObject->Begin();

				CLevelMgr::GetInst()->m_LevelChanged = true;
			}
		}
		break;
		case TASK_TYPE::DELETE_OBJECT:
		{
			CGameObject* pObject = (CGameObject*)task.Param_0;
			if (pObject->m_Dead)
			{
				continue;
			}

			// GC �� �ֱ�
			pObject->m_Dead = true;			
			m_GC.push_back(pObject);

			CLevelMgr::GetInst()->m_LevelChanged = true;
		}
		break;

		case TASK_TYPE::CHANGE_LEVELSTATE:
		{
			LEVEL_STATE NextState = (LEVEL_STATE)task.Param_0;
			CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
			pCurLevel->ChangeState(NextState);
		}
			break;

		case TASK_TYPE::CHANGE_LEVEL:
		{
			// Param_0 : Level Adress, Param_1 : Level State
			CLevel* pLevel = (CLevel*)task.Param_0;
			LEVEL_STATE NextState = (LEVEL_STATE)task.Param_1;

			CLevelMgr::GetInst()->ChangeStopLevel(pLevel);
			pLevel->ChangeState(NextState);
		}
			break;

		case TASK_TYPE::ASSET_CHANGED:
		{
			CAssetMgr::GetInst()->m_Changed = true;
		}

		case TASK_TYPE::LEVEL_CHANGED:
		{
			CLevelMgr::GetInst()->m_LevelChanged = true;
		}
		break;

		case TASK_TYPE::DEL_ASSET:
		{
			CAsset* pAsset = (CAsset*)task.Param_0;
			ASSET_TYPE Type = pAsset->GetAssetType();
			CAssetMgr::GetInst()->DeleteAsset(Type, pAsset->GetKey());
		}
		break;
		case TASK_TYPE::ADD_CHILD:
		{
			CGameObject* pParent = (CGameObject*)task.Param_0;
			CGameObject* pChild = (CGameObject*)task.Param_1;
			pParent->AddChild(pChild);

			CLevelMgr::GetInst()->m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::DEL_COMPONENT:
		{
			CGameObject* pObj = (CGameObject*)task.Param_0;
			COMPONENT_TYPE type = (COMPONENT_TYPE)task.Param_1;

			pObj->DeregisterComponent(type);

			CLevelMgr::GetInst()->m_LevelChanged = true;
		}
		break;
		case TASK_TYPE::DEL_SCRIPT:
		{
			CGameObject* pObj = (CGameObject*)task.Param_0;
			CScript* script = (CScript*)task.Param_1;
			
			pObj->DeregisterComponent(COMPONENT_TYPE::SCRIPT, script);
			
			CLevelMgr::GetInst()->m_LevelChanged = true;
		}
		break;
		}
	}

	m_vecTask.clear();	
}