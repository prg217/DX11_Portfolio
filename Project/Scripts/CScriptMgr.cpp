#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CCameraPlayerTrackingScript.h"
#include "CCountDownDeleteScript.h"
#include "CGlowingPlantsScript.h"
#include "CGrassScript.h"
#include "CInteractionScript.h"
#include "CJellyPushScript.h"
#include "CLiftScript.h"
#include "CMissileScript.h"
#include "CNPCScript.h"
#include "COguDancePointLightScript.h"
#include "CPlayerCameraScript.h"
#include "CPlayerInteractionScript.h"
#include "CPlayerScript.h"
#include "CPushScript.h"
#include "CSwingObjScript.h"
#include "CTextBoxScript.h"
#include "CUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCameraPlayerTrackingScript");
	_vec.push_back(L"CCountDownDeleteScript");
	_vec.push_back(L"CGlowingPlantsScript");
	_vec.push_back(L"CGrassScript");
	_vec.push_back(L"CInteractionScript");
	_vec.push_back(L"CJellyPushScript");
	_vec.push_back(L"CLiftScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CNPCScript");
	_vec.push_back(L"COguDancePointLightScript");
	_vec.push_back(L"CPlayerCameraScript");
	_vec.push_back(L"CPlayerInteractionScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPushScript");
	_vec.push_back(L"CSwingObjScript");
	_vec.push_back(L"CTextBoxScript");
	_vec.push_back(L"CUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCameraPlayerTrackingScript" == _strScriptName)
		return new CCameraPlayerTrackingScript;
	if (L"CCountDownDeleteScript" == _strScriptName)
		return new CCountDownDeleteScript;
	if (L"CGlowingPlantsScript" == _strScriptName)
		return new CGlowingPlantsScript;
	if (L"CGrassScript" == _strScriptName)
		return new CGrassScript;
	if (L"CInteractionScript" == _strScriptName)
		return new CInteractionScript;
	if (L"CJellyPushScript" == _strScriptName)
		return new CJellyPushScript;
	if (L"CLiftScript" == _strScriptName)
		return new CLiftScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CNPCScript" == _strScriptName)
		return new CNPCScript;
	if (L"COguDancePointLightScript" == _strScriptName)
		return new COguDancePointLightScript;
	if (L"CPlayerCameraScript" == _strScriptName)
		return new CPlayerCameraScript;
	if (L"CPlayerInteractionScript" == _strScriptName)
		return new CPlayerInteractionScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPushScript" == _strScriptName)
		return new CPushScript;
	if (L"CSwingObjScript" == _strScriptName)
		return new CSwingObjScript;
	if (L"CTextBoxScript" == _strScriptName)
		return new CTextBoxScript;
	if (L"CUIScript" == _strScriptName)
		return new CUIScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAPLAYERTRACKINGSCRIPT:
		return new CCameraPlayerTrackingScript;
		break;
	case (UINT)SCRIPT_TYPE::COUNTDOWNDELETESCRIPT:
		return new CCountDownDeleteScript;
		break;
	case (UINT)SCRIPT_TYPE::GLOWINGPLANTSSCRIPT:
		return new CGlowingPlantsScript;
		break;
	case (UINT)SCRIPT_TYPE::GRASSSCRIPT:
		return new CGrassScript;
		break;
	case (UINT)SCRIPT_TYPE::INTERACTIONSCRIPT:
		return new CInteractionScript;
		break;
	case (UINT)SCRIPT_TYPE::JELLYPUSHSCRIPT:
		return new CJellyPushScript;
		break;
	case (UINT)SCRIPT_TYPE::LIFTSCRIPT:
		return new CLiftScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::NPCSCRIPT:
		return new CNPCScript;
		break;
	case (UINT)SCRIPT_TYPE::OGUDANCEPOINTLIGHTSCRIPT:
		return new COguDancePointLightScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERCAMERASCRIPT:
		return new CPlayerCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERINTERACTIONSCRIPT:
		return new CPlayerInteractionScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PUSHSCRIPT:
		return new CPushScript;
		break;
	case (UINT)SCRIPT_TYPE::SWINGOBJSCRIPT:
		return new CSwingObjScript;
		break;
	case (UINT)SCRIPT_TYPE::TEXTBOXSCRIPT:
		return new CTextBoxScript;
		break;
	case (UINT)SCRIPT_TYPE::UISCRIPT:
		return new CUIScript;
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

	case SCRIPT_TYPE::CAMERAPLAYERTRACKINGSCRIPT:
		return L"CCameraPlayerTrackingScript";
		break;

	case SCRIPT_TYPE::COUNTDOWNDELETESCRIPT:
		return L"CCountDownDeleteScript";
		break;

	case SCRIPT_TYPE::GLOWINGPLANTSSCRIPT:
		return L"CGlowingPlantsScript";
		break;

	case SCRIPT_TYPE::GRASSSCRIPT:
		return L"CGrassScript";
		break;

	case SCRIPT_TYPE::INTERACTIONSCRIPT:
		return L"CInteractionScript";
		break;

	case SCRIPT_TYPE::JELLYPUSHSCRIPT:
		return L"CJellyPushScript";
		break;

	case SCRIPT_TYPE::LIFTSCRIPT:
		return L"CLiftScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::NPCSCRIPT:
		return L"CNPCScript";
		break;

	case SCRIPT_TYPE::OGUDANCEPOINTLIGHTSCRIPT:
		return L"COguDancePointLightScript";
		break;

	case SCRIPT_TYPE::PLAYERCAMERASCRIPT:
		return L"CPlayerCameraScript";
		break;

	case SCRIPT_TYPE::PLAYERINTERACTIONSCRIPT:
		return L"CPlayerInteractionScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PUSHSCRIPT:
		return L"CPushScript";
		break;

	case SCRIPT_TYPE::SWINGOBJSCRIPT:
		return L"CSwingObjScript";
		break;

	case SCRIPT_TYPE::TEXTBOXSCRIPT:
		return L"CTextBoxScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"CUIScript";
		break;

	}
	return nullptr;
}