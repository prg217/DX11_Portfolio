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
	// �ִ� ���ۿ� ������� �ڵ����� ����� ����

	// �̴� ���� �� ���
	// ���� ���̸� ���� �з���
	// �ٸ� ���̸� �� ���� ���� �ռ���(���� ��ġ ��� �������� ���ٰ� ������)
	CScript* script = _OtherObject->GetScript("CJellyPushScript");
	CJellyPushScript* jellyPushScript = dynamic_cast<CJellyPushScript*>(script);

	if (jellyPushScript != nullptr)
	{
		// �ڽ��� �̹� �������� �����ߴٸ� return
		if (m_IsOverlap)
		{
			return;
		}
		// ������ �̹� ������ �ڵ带 �����ϰ� ������ return
		if (jellyPushScript->IsOverlap())
		{
			// ���� �ε�ģ ������ ���ٰ� ������� �Լ��� �־���� ��
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

	// ū ���� �� ���
	// ��� �и��Ǿ� ���� 1���� �÷��̾ ��� ��
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
