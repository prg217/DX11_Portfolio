#include "pch.h"
#include "CTestLevelSoundScript.h"

CTestLevelSoundScript::CTestLevelSoundScript()
	: CScript(UINT(SCRIPT_TYPE::TESTLEVELSOUNDSCRIPT))
{
}

CTestLevelSoundScript::CTestLevelSoundScript(const CTestLevelSoundScript& _Origin)
	: CScript(_Origin)
{
}

CTestLevelSoundScript::~CTestLevelSoundScript()
{
}

void CTestLevelSoundScript::Begin()
{
	// Sound Àç»ý
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\BGM_10_MoonForestAmbience.wav");
	pSound->Play(0, 0.5f, false);
}

void CTestLevelSoundScript::Tick()
{
}

void CTestLevelSoundScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CTestLevelSoundScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CTestLevelSoundScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CTestLevelSoundScript::SaveToFile(FILE* _File)
{
}

void CTestLevelSoundScript::LoadFromFile(FILE* _File)
{
}
