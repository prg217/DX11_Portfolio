#include "pch.h"
#include "CScriptMgr.h"

#include "CBugNPCScript.h"
#include "CCactusNeedleScript.h"
#include "CCameraMoveScript.h"
#include "CCameraPlayerTrackingScript.h"
#include "CCountDownDeleteScript.h"
#include "CFlowerLightAreaScript.h"
#include "CFlowerLightScript.h"
#include "CGlowingPlantsScript.h"
#include "CGrassScript.h"
#include "CHPScript.h"
#include "CInteractionScript.h"
#include "CJellyBombDetectScript.h"
#include "CJellyBombScript.h"
#include "CJellyPushFrameScript.h"
#include "CJellyPushScript.h"
#include "CLiftScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CNPCScript.h"
#include "COguDancePointLightScript.h"
#include "CPlayerCameraScript.h"
#include "CPlayerDetectScript.h"
#include "CPlayerInteractionScript.h"
#include "CPlayerScript.h"
#include "CPushScript.h"
#include "CSpitCactusScript.h"
#include "CStoneBreakableScript.h"
#include "CSwingObjScript.h"
#include "CTextBoxScript.h"
#include "CUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBugNPCScript");
	_vec.push_back(L"CCactusNeedleScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCameraPlayerTrackingScript");
	_vec.push_back(L"CCountDownDeleteScript");
	_vec.push_back(L"CFlowerLightAreaScript");
	_vec.push_back(L"CFlowerLightScript");
	_vec.push_back(L"CGlowingPlantsScript");
	_vec.push_back(L"CGrassScript");
	_vec.push_back(L"CHPScript");
	_vec.push_back(L"CInteractionScript");
	_vec.push_back(L"CJellyBombDetectScript");
	_vec.push_back(L"CJellyBombScript");
	_vec.push_back(L"CJellyPushFrameScript");
	_vec.push_back(L"CJellyPushScript");
	_vec.push_back(L"CLiftScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CNPCScript");
	_vec.push_back(L"COguDancePointLightScript");
	_vec.push_back(L"CPlayerCameraScript");
	_vec.push_back(L"CPlayerDetectScript");
	_vec.push_back(L"CPlayerInteractionScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPushScript");
	_vec.push_back(L"CSpitCactusScript");
	_vec.push_back(L"CStoneBreakableScript");
	_vec.push_back(L"CSwingObjScript");
	_vec.push_back(L"CTextBoxScript");
	_vec.push_back(L"CUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBugNPCScript" == _strScriptName)
		return new CBugNPCScript;
	if (L"CCactusNeedleScript" == _strScriptName)
		return new CCactusNeedleScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCameraPlayerTrackingScript" == _strScriptName)
		return new CCameraPlayerTrackingScript;
	if (L"CCountDownDeleteScript" == _strScriptName)
		return new CCountDownDeleteScript;
	if (L"CFlowerLightAreaScript" == _strScriptName)
		return new CFlowerLightAreaScript;
	if (L"CFlowerLightScript" == _strScriptName)
		return new CFlowerLightScript;
	if (L"CGlowingPlantsScript" == _strScriptName)
		return new CGlowingPlantsScript;
	if (L"CGrassScript" == _strScriptName)
		return new CGrassScript;
	if (L"CHPScript" == _strScriptName)
		return new CHPScript;
	if (L"CInteractionScript" == _strScriptName)
		return new CInteractionScript;
	if (L"CJellyBombDetectScript" == _strScriptName)
		return new CJellyBombDetectScript;
	if (L"CJellyBombScript" == _strScriptName)
		return new CJellyBombScript;
	if (L"CJellyPushFrameScript" == _strScriptName)
		return new CJellyPushFrameScript;
	if (L"CJellyPushScript" == _strScriptName)
		return new CJellyPushScript;
	if (L"CLiftScript" == _strScriptName)
		return new CLiftScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CNPCScript" == _strScriptName)
		return new CNPCScript;
	if (L"COguDancePointLightScript" == _strScriptName)
		return new COguDancePointLightScript;
	if (L"CPlayerCameraScript" == _strScriptName)
		return new CPlayerCameraScript;
	if (L"CPlayerDetectScript" == _strScriptName)
		return new CPlayerDetectScript;
	if (L"CPlayerInteractionScript" == _strScriptName)
		return new CPlayerInteractionScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CPushScript" == _strScriptName)
		return new CPushScript;
	if (L"CSpitCactusScript" == _strScriptName)
		return new CSpitCactusScript;
	if (L"CStoneBreakableScript" == _strScriptName)
		return new CStoneBreakableScript;
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
	case (UINT)SCRIPT_TYPE::BUGNPCSCRIPT:
		return new CBugNPCScript;
		break;
	case (UINT)SCRIPT_TYPE::CACTUSNEEDLESCRIPT:
		return new CCactusNeedleScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAPLAYERTRACKINGSCRIPT:
		return new CCameraPlayerTrackingScript;
		break;
	case (UINT)SCRIPT_TYPE::COUNTDOWNDELETESCRIPT:
		return new CCountDownDeleteScript;
		break;
	case (UINT)SCRIPT_TYPE::FLOWERLIGHTAREASCRIPT:
		return new CFlowerLightAreaScript;
		break;
	case (UINT)SCRIPT_TYPE::FLOWERLIGHTSCRIPT:
		return new CFlowerLightScript;
		break;
	case (UINT)SCRIPT_TYPE::GLOWINGPLANTSSCRIPT:
		return new CGlowingPlantsScript;
		break;
	case (UINT)SCRIPT_TYPE::GRASSSCRIPT:
		return new CGrassScript;
		break;
	case (UINT)SCRIPT_TYPE::HPSCRIPT:
		return new CHPScript;
		break;
	case (UINT)SCRIPT_TYPE::INTERACTIONSCRIPT:
		return new CInteractionScript;
		break;
	case (UINT)SCRIPT_TYPE::JELLYBOMBDETECTSCRIPT:
		return new CJellyBombDetectScript;
		break;
	case (UINT)SCRIPT_TYPE::JELLYBOMBSCRIPT:
		return new CJellyBombScript;
		break;
	case (UINT)SCRIPT_TYPE::JELLYPUSHFRAMESCRIPT:
		return new CJellyPushFrameScript;
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
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
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
	case (UINT)SCRIPT_TYPE::PLAYERDETECTSCRIPT:
		return new CPlayerDetectScript;
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
	case (UINT)SCRIPT_TYPE::SPITCACTUSSCRIPT:
		return new CSpitCactusScript;
		break;
	case (UINT)SCRIPT_TYPE::STONEBREAKABLESCRIPT:
		return new CStoneBreakableScript;
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
	case SCRIPT_TYPE::BUGNPCSCRIPT:
		return L"CBugNPCScript";
		break;

	case SCRIPT_TYPE::CACTUSNEEDLESCRIPT:
		return L"CCactusNeedleScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::CAMERAPLAYERTRACKINGSCRIPT:
		return L"CCameraPlayerTrackingScript";
		break;

	case SCRIPT_TYPE::COUNTDOWNDELETESCRIPT:
		return L"CCountDownDeleteScript";
		break;

	case SCRIPT_TYPE::FLOWERLIGHTAREASCRIPT:
		return L"CFlowerLightAreaScript";
		break;

	case SCRIPT_TYPE::FLOWERLIGHTSCRIPT:
		return L"CFlowerLightScript";
		break;

	case SCRIPT_TYPE::GLOWINGPLANTSSCRIPT:
		return L"CGlowingPlantsScript";
		break;

	case SCRIPT_TYPE::GRASSSCRIPT:
		return L"CGrassScript";
		break;

	case SCRIPT_TYPE::HPSCRIPT:
		return L"CHPScript";
		break;

	case SCRIPT_TYPE::INTERACTIONSCRIPT:
		return L"CInteractionScript";
		break;

	case SCRIPT_TYPE::JELLYBOMBDETECTSCRIPT:
		return L"CJellyBombDetectScript";
		break;

	case SCRIPT_TYPE::JELLYBOMBSCRIPT:
		return L"CJellyBombScript";
		break;

	case SCRIPT_TYPE::JELLYPUSHFRAMESCRIPT:
		return L"CJellyPushFrameScript";
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

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
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

	case SCRIPT_TYPE::PLAYERDETECTSCRIPT:
		return L"CPlayerDetectScript";
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

	case SCRIPT_TYPE::SPITCACTUSSCRIPT:
		return L"CSpitCactusScript";
		break;

	case SCRIPT_TYPE::STONEBREAKABLESCRIPT:
		return L"CStoneBreakableScript";
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