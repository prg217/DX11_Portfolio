#include "pch.h"
#include "CScriptMgr.h"

#include "CAniFinishDestroyScript.h"
#include "CBossHPScript.h"
#include "CBossInScript.h"
#include "CBugBallScript.h"
#include "CBugBossScript.h"
#include "CBugNPCScript.h"
#include "CBugRollingScript.h"
#include "CCactusNeedleScript.h"
#include "CCameraMoveScript.h"
#include "CCameraPlayerTrackingScript.h"
#include "CColorBugScript.h"
#include "CCountDownDeleteScript.h"
#include "CDoorOpenScript.h"
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
#include "CLevelChangeScript.h"
#include "CLiftScript.h"
#include "CLightBallScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CNPCScript.h"
#include "COguDancePointLightScript.h"
#include "CPlayerCameraScript.h"
#include "CPlayerDetectScript.h"
#include "CPlayerHPScript.h"
#include "CPlayerInteractionScript.h"
#include "CPlayerScript.h"
#include "CPushScript.h"
#include "CSpitCactusScript.h"
#include "CStoneBreakableScript.h"
#include "CSwingObjScript.h"
#include "CSymbolScript.h"
#include "CTestLevelSoundScript.h"
#include "CTextBoxScript.h"
#include "CUIScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAniFinishDestroyScript");
	_vec.push_back(L"CBossHPScript");
	_vec.push_back(L"CBossInScript");
	_vec.push_back(L"CBugBallScript");
	_vec.push_back(L"CBugBossScript");
	_vec.push_back(L"CBugNPCScript");
	_vec.push_back(L"CBugRollingScript");
	_vec.push_back(L"CCactusNeedleScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CCameraPlayerTrackingScript");
	_vec.push_back(L"CColorBugScript");
	_vec.push_back(L"CCountDownDeleteScript");
	_vec.push_back(L"CDoorOpenScript");
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
	_vec.push_back(L"CLevelChangeScript");
	_vec.push_back(L"CLiftScript");
	_vec.push_back(L"CLightBallScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CNPCScript");
	_vec.push_back(L"COguDancePointLightScript");
	_vec.push_back(L"CPlayerCameraScript");
	_vec.push_back(L"CPlayerDetectScript");
	_vec.push_back(L"CPlayerHPScript");
	_vec.push_back(L"CPlayerInteractionScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CPushScript");
	_vec.push_back(L"CSpitCactusScript");
	_vec.push_back(L"CStoneBreakableScript");
	_vec.push_back(L"CSwingObjScript");
	_vec.push_back(L"CSymbolScript");
	_vec.push_back(L"CTestLevelSoundScript");
	_vec.push_back(L"CTextBoxScript");
	_vec.push_back(L"CUIScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAniFinishDestroyScript" == _strScriptName)
		return new CAniFinishDestroyScript;
	if (L"CBossHPScript" == _strScriptName)
		return new CBossHPScript;
	if (L"CBossInScript" == _strScriptName)
		return new CBossInScript;
	if (L"CBugBallScript" == _strScriptName)
		return new CBugBallScript;
	if (L"CBugBossScript" == _strScriptName)
		return new CBugBossScript;
	if (L"CBugNPCScript" == _strScriptName)
		return new CBugNPCScript;
	if (L"CBugRollingScript" == _strScriptName)
		return new CBugRollingScript;
	if (L"CCactusNeedleScript" == _strScriptName)
		return new CCactusNeedleScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CCameraPlayerTrackingScript" == _strScriptName)
		return new CCameraPlayerTrackingScript;
	if (L"CColorBugScript" == _strScriptName)
		return new CColorBugScript;
	if (L"CCountDownDeleteScript" == _strScriptName)
		return new CCountDownDeleteScript;
	if (L"CDoorOpenScript" == _strScriptName)
		return new CDoorOpenScript;
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
	if (L"CLevelChangeScript" == _strScriptName)
		return new CLevelChangeScript;
	if (L"CLiftScript" == _strScriptName)
		return new CLiftScript;
	if (L"CLightBallScript" == _strScriptName)
		return new CLightBallScript;
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
	if (L"CPlayerHPScript" == _strScriptName)
		return new CPlayerHPScript;
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
	if (L"CSymbolScript" == _strScriptName)
		return new CSymbolScript;
	if (L"CTestLevelSoundScript" == _strScriptName)
		return new CTestLevelSoundScript;
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
	case (UINT)SCRIPT_TYPE::ANIFINISHDESTROYSCRIPT:
		return new CAniFinishDestroyScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSHPSCRIPT:
		return new CBossHPScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSINSCRIPT:
		return new CBossInScript;
		break;
	case (UINT)SCRIPT_TYPE::BUGBALLSCRIPT:
		return new CBugBallScript;
		break;
	case (UINT)SCRIPT_TYPE::BUGBOSSSCRIPT:
		return new CBugBossScript;
		break;
	case (UINT)SCRIPT_TYPE::BUGNPCSCRIPT:
		return new CBugNPCScript;
		break;
	case (UINT)SCRIPT_TYPE::BUGROLLINGSCRIPT:
		return new CBugRollingScript;
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
	case (UINT)SCRIPT_TYPE::COLORBUGSCRIPT:
		return new CColorBugScript;
		break;
	case (UINT)SCRIPT_TYPE::COUNTDOWNDELETESCRIPT:
		return new CCountDownDeleteScript;
		break;
	case (UINT)SCRIPT_TYPE::DOOROPENSCRIPT:
		return new CDoorOpenScript;
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
	case (UINT)SCRIPT_TYPE::LEVELCHANGESCRIPT:
		return new CLevelChangeScript;
		break;
	case (UINT)SCRIPT_TYPE::LIFTSCRIPT:
		return new CLiftScript;
		break;
	case (UINT)SCRIPT_TYPE::LIGHTBALLSCRIPT:
		return new CLightBallScript;
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
	case (UINT)SCRIPT_TYPE::PLAYERHPSCRIPT:
		return new CPlayerHPScript;
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
	case (UINT)SCRIPT_TYPE::SYMBOLSCRIPT:
		return new CSymbolScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTLEVELSOUNDSCRIPT:
		return new CTestLevelSoundScript;
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
	case SCRIPT_TYPE::ANIFINISHDESTROYSCRIPT:
		return L"CAniFinishDestroyScript";
		break;

	case SCRIPT_TYPE::BOSSHPSCRIPT:
		return L"CBossHPScript";
		break;

	case SCRIPT_TYPE::BOSSINSCRIPT:
		return L"CBossInScript";
		break;

	case SCRIPT_TYPE::BUGBALLSCRIPT:
		return L"CBugBallScript";
		break;

	case SCRIPT_TYPE::BUGBOSSSCRIPT:
		return L"CBugBossScript";
		break;

	case SCRIPT_TYPE::BUGNPCSCRIPT:
		return L"CBugNPCScript";
		break;

	case SCRIPT_TYPE::BUGROLLINGSCRIPT:
		return L"CBugRollingScript";
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

	case SCRIPT_TYPE::COLORBUGSCRIPT:
		return L"CColorBugScript";
		break;

	case SCRIPT_TYPE::COUNTDOWNDELETESCRIPT:
		return L"CCountDownDeleteScript";
		break;

	case SCRIPT_TYPE::DOOROPENSCRIPT:
		return L"CDoorOpenScript";
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

	case SCRIPT_TYPE::LEVELCHANGESCRIPT:
		return L"CLevelChangeScript";
		break;

	case SCRIPT_TYPE::LIFTSCRIPT:
		return L"CLiftScript";
		break;

	case SCRIPT_TYPE::LIGHTBALLSCRIPT:
		return L"CLightBallScript";
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

	case SCRIPT_TYPE::PLAYERHPSCRIPT:
		return L"CPlayerHPScript";
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

	case SCRIPT_TYPE::SYMBOLSCRIPT:
		return L"CSymbolScript";
		break;

	case SCRIPT_TYPE::TESTLEVELSOUNDSCRIPT:
		return L"CTestLevelSoundScript";
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