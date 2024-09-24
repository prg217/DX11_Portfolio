#include "pch.h"
#include "CLevelChangeScript.h"

CLevelChangeScript::CLevelChangeScript()
	: CScript(UINT(SCRIPT_TYPE::LEVELCHANGESCRIPT))
	, m_LeveName(L"")
{
}

CLevelChangeScript::CLevelChangeScript(const CLevelChangeScript& _Origin)
	: CScript(_Origin)
	, m_LeveName(L"")
{
}

CLevelChangeScript::~CLevelChangeScript()
{
}

void CLevelChangeScript::Begin()
{
	// 이동하려는 레벨의 이름을 게임 오브젝트 이름으로 해놓는다.
	m_LeveName = GetOwner()->GetName();
	m_LeveName.erase(std::find(m_LeveName.begin(), m_LeveName.end(), L'\0'), m_LeveName.end());
}

void CLevelChangeScript::Tick()
{
}

void CLevelChangeScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == 3)
	{
		NextLevel();
	}
}

void CLevelChangeScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLevelChangeScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CLevelChangeScript::SaveToFile(FILE* _File)
{
	//fwrite(&m_LeveName, sizeof(wstring), 1, _File);
}

void CLevelChangeScript::LoadFromFile(FILE* _File)
{
	//fread(&m_LeveName, sizeof(wstring), 1, _File);
}

void CLevelChangeScript::NextLevel()
{
	CLevelMgr::GetInst()->LevelNameChange(m_LeveName);
}

