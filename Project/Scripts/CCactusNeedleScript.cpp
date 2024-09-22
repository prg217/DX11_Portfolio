#include "pch.h"
#include "CCactusNeedleScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CSpitCactusScript.h"

#include "CPlayerScript.h"
#include "CAniFinishDestroyScript.h"

CCactusNeedleScript::CCactusNeedleScript()
	: CScript(UINT(SCRIPT_TYPE::CACTUSNEEDLESCRIPT))
	, m_Type(SpitCactusType::Nomal)
	, m_Dir(NeedleDir::UP)
	, m_Speed(200.f)
	, m_SaveTime(0.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Type", &m_Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Dir", &m_Dir);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &m_Speed);
}

CCactusNeedleScript::CCactusNeedleScript(const CCactusNeedleScript& _Origin)
	: CScript(_Origin)
	, m_Type(_Origin.m_Type)
	, m_Dir(_Origin.m_Dir)
	, m_Speed(200.f)
	, m_SaveTime(0.f)
{
}

CCactusNeedleScript::~CCactusNeedleScript()
{
}

void CCactusNeedleScript::Begin()
{
	// 레이어 바꾸기
	int LayerIdx = GetOwner()->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);

	pLayer->LayerChange(GetOwner(), 10);

	// Poison일 경우 독 파티클 생성
	if (m_Type == SpitCactusType::Poison)
	{
		PoisonEffect();
	}
}

void CCactusNeedleScript::Tick()
{
	// 3초 후 삭제
	m_SaveTime += DT;
	if (m_SaveTime >= 3.f)
	{
		DeleteObject(GetOwner());
	}

	Vec3 pos = GetOwner()->Transform()->GetRelativePos();

	switch (m_Dir)
	{
	case NeedleDir::UP:
		pos.y += DT * m_Speed;
		break;
	case NeedleDir::DOWN:
		pos.y -= DT * m_Speed;
		break;
	case NeedleDir::LEFT:
		pos.x -= DT * m_Speed;
		break;
	case NeedleDir::RIGHT:
		pos.x += DT * m_Speed;
		break;
	default:
		break;
	}

	GetOwner()->Transform()->SetRelativePos(pos);
}

void CCactusNeedleScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 플레이어에게 Hit보내기
	if (_OtherObject->GetLayerIdx() == 3)
	{
		if (_OtherObject->GetScript("CPlayerScript") != nullptr)
		{
			CPlayerScript* script = dynamic_cast<CPlayerScript*>(_OtherObject->GetScript("CPlayerScript"));
			script->Hit();

			switch (m_Type)
			{
			case SpitCactusType::Poison:
				// 독 상태이상 부여
				break;
			default:
				break;
			}

			DeleteObject(GetOwner());
		}
	}
	// 벽, 상호작용에 부딪치면 삭제
	else if (_OtherObject->GetLayerIdx() == 28 || _OtherObject->GetLayerIdx() == 29 || _OtherObject->GetLayerIdx() == 6)
	{
		AttackEffect();
		DeleteObject(GetOwner());
	}
}

void CCactusNeedleScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCactusNeedleScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CCactusNeedleScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(SpitCactusType), 1, _File);
	fwrite(&m_Speed, sizeof(float), 1, _File); 
	fwrite(&m_Dir, sizeof(NeedleDir), 1, _File);
}

void CCactusNeedleScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(SpitCactusType), 1, _File);
	fread(&m_Speed, sizeof(float), 1, _File);
	fread(&m_Dir, sizeof(NeedleDir), 1, _File);
}

void CCactusNeedleScript::AttackEffect()
{
	CGameObject* effect = new CGameObject;
	effect->SetName(L"AttackEffect");

	effect->AddComponent(new CTransform);
	effect->AddComponent(new CMeshRender);
	effect->AddComponent(new CFlipBookComponent);
	effect->AddComponent(new CAniFinishDestroyScript);

	effect->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	effect->Transform()->SetRelativeScale(Vec3(100, 100, 1));

	effect->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	effect->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl"));

	Ptr<CFlipBook> pFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Effect\\hit.flip");
	effect->FlipBookComponent()->AddFlipBook(0, pFlip);
	effect->FlipBookComponent()->Play(0, 8, false);

	CreateObject(effect, 0);
}

void CCactusNeedleScript::PoisonEffect()
{
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"PoisonParticle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	// 파티클의 방향은 파티클들이 향하는 방향이다.
	switch (m_Dir)
	{
	case NeedleDir::UP:
		strInitPath += L"particle\\monster_poison_down.particle";
		break;
	case NeedleDir::DOWN:
		strInitPath += L"particle\\monster_poison_up.particle";
		break;
	case NeedleDir::LEFT:
		strInitPath += L"particle\\monster_poison_right.particle";
		break;
	case NeedleDir::RIGHT:
		strInitPath += L"particle\\monster_poison_left.particle";
		break;
	default:
		break;
	}

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	pParticleObj->ParticleSystem()->LoadFromFile(File);
	fclose(File);

	pParticleObj->Transform()->SetRelativePos(Vec3(0, 0, 0));

	CreateObject(pParticleObj, 0);
	AddChildObject(GetOwner(), pParticleObj);
}
