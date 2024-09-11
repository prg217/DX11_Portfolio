#include "pch.h"
#include "CJellyPushScript.h"

#include "CInteractionScript.h"
#include "CPushScript.h"

CJellyPushScript::CJellyPushScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHSCRIPT))
	, m_IsOverlap(false)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_Type);
}

CJellyPushScript::CJellyPushScript(const CJellyPushScript& _Origin)
	: CScript(_Origin)
	, m_IsOverlap(false)
{
}

CJellyPushScript::~CJellyPushScript()
{
}

void CJellyPushScript::Begin()
{
}

void CJellyPushScript::Tick()
{
}

void CJellyPushScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// 넣는 구멍에 닿았으면 자동으로 가운데에 안착

	// 미니 젤리 일 경우
	// 같은 색이면 같이 밀려짐
	// 다른 색이면 그 색에 따라 합성됨(서로 위치 가운데 지점으로 가다가 합쳐짐)
	CScript* script = _OtherObject->GetScript("CJellyPushScript");
	CJellyPushScript* jellyPushScript = dynamic_cast<CJellyPushScript*>(script);

	if (jellyPushScript != nullptr)
	{
		// 자신이 이미 오버랩을 수행했다면 return
		if (m_IsOverlap)
		{
			return;
		}
		// 상대방이 이미 오버랩 코드를 실행하고 있으면 return
		if (jellyPushScript->IsOverlap())
		{
			// 이제 부딪친 곳으로 가다가 사라지는 함수는 넣어줘야 함
			return;
		}

		m_IsOverlap = true;
		switch (jellyPushScript->GetJellyPushType())
		{
		case JellyPushType::CYAN:
			switch (m_Type)
			{
			case JellyPushType::MAGENTA:
				CreateBlue();
				DeleteObject(GetOwner());
				break;
			case JellyPushType::YELLOW:
				break;
			default:
				break;
			}
			break;
		case JellyPushType::MAGENTA:
			switch (m_Type)
			{
			case JellyPushType::CYAN:
				CreateBlue();
				DeleteObject(GetOwner());
				break;
			case JellyPushType::YELLOW:
				break;
			default:
				break;
			}
			break;
		case JellyPushType::YELLOW:
			switch (m_Type)
			{
			case JellyPushType::CYAN:
				break;
			case JellyPushType::MAGENTA:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	// 큰 젤리 일 경우
	// 들면 분리되어 랜덤 1개를 플레이어가 들게 됨
}

void CJellyPushScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(JellyPushType), 1, _File);
}

void CJellyPushScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(JellyPushType), 1, _File);
}

void CJellyPushScript::CreateBlue()
{
	CGameObject* jelly = new CGameObject;
	jelly->SetName(L"JellyPush_Blue");
	jelly->AddComponent(new CTransform);
	jelly->AddComponent(new CCollider2D);
	jelly->AddComponent(new CMeshRender);
	jelly->AddComponent(new CSpriteComponent);
	jelly->AddComponent(new CRigidBody);
	jelly->AddComponent(new CInteractionScript);
	jelly->AddComponent(new CPushScript);

	jelly->Transform()->SetIndependentScale(true);
	jelly->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	jelly->Transform()->SetRelativeScale(Vec3(120.f, 120.f, 0.f));

	jelly->Collider2D()->SetIndependentScale(false);
	jelly->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	jelly->Collider2D()->SetScale(Vec3(0.4f, 0.2f, 1.f));

	jelly->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	jelly->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\obstacle\\jellyPush_blue.sprite");
	jelly->SpriteComponent()->AddSprite(pSprite);

	CreateObject(jelly, 6);
}
