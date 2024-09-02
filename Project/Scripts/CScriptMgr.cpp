#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CCountDownDeleteScript.h"
#include "CInteractionScript.h"
#include "CLiftScript.h"
#include "CMissileScript.h"
#include "COguDancePointLightScript.h"
#include "CPlayerInteractionScript.h"
#include "CPlayerScript.h"
#include "CSwingObjScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCountDownDeleteScript");
	_vec.push_back(L"CInteractionScript");
	_vec.push_back(L"CLiftScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"COguDancePointLightScript");
	_vec.push_back(L"CPlayerInteractionScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CSwingObjScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCountDownDeleteScript" == _strScriptName)
		return new CCountDownDeleteScript;
	if (L"CInteractionScript" == _strScriptName)
		return new CInteractionScript;
	if (L"CLiftScript" == _strScriptName)
		return new CLiftScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"COguDancePointLightScript" == _strScriptName)
		return new COguDancePointLightScript;
	if (L"CPlayerInteractionScript" == _strScriptName)
		return new CPlayerInteractionScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CSwingObjScript" == _strScriptName)
		return new CSwingObjScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::COUNTDOWNDELETESCRIPT:
		return new CCountDownDeleteScript;
		break;
	case (UINT)SCRIPT_TYPE::INTERACTIONSCRIPT:
		return new CInteractionScript;
		break;
	case (UINT)SCRIPT_TYPE::LIFTSCRIPT:
		return new CLiftScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::OGUDANCEPOINTLIGHTSCRIPT:
		return new COguDancePointLightScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERINTERACTIONSCRIPT:
		return new CPlayerInteractionScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SWINGOBJSCRIPT:
		return new CSwingObjScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::COUNTDOWNDELETESCRIPT:
		return L"CCountDownDeleteScript";
		break;

	case SCRIPT_TYPE::INTERACTIONSCRIPT:
		return L"CInteractionScript";
		break;

	case SCRIPT_TYPE::LIFTSCRIPT:
		return L"CLiftScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::OGUDANCEPOINTLIGHTSCRIPT:
		return L"COguDancePointLightScript";
		break;

	case SCRIPT_TYPE::PLAYERINTERACTIONSCRIPT:
		return L"CPlayerInteractionScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SWINGOBJSCRIPT:
		return L"CSwingObjScript";
		break;

	}
	return nullptr;
}