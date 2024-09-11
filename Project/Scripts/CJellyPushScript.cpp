#include "pch.h"
#include "CJellyPushScript.h"

#include "CInteractionScript.h"
#include "CPushScript.h"
#include "CCountDownDeleteScript.h"

CJellyPushScript::CJellyPushScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHSCRIPT))
	, m_Type(JellyPushType::NONE)
	, m_OtherObjType(JellyPushType::NONE)
	, m_IsOverlap(false)
	, m_Destination(nullptr)
	, m_Speed(100.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_Type);
}

CJellyPushScript::CJellyPushScript(const CJellyPushScript& _Origin)
	: CScript(_Origin)
	, m_Type(JellyPushType::NONE)
	, m_OtherObjType(JellyPushType::NONE)
	, m_IsOverlap(false)
	, m_Destination(nullptr)
	, m_Speed(100.f)
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
	DestinationMove();

	if (GetOwner()->IsDead())
	{
		switch (m_OtherObjType)
		{
		case JellyPushType::CYAN:
			switch (m_Type)
			{
			case JellyPushType::MAGENTA:
				CreateBlue();
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

		// _OtherObject�� ���� �̵�
		SetDestinationMove(_OtherObject);

		// ������ �̹� ������ �ڵ带 �����ϰ� ������ return
		if (jellyPushScript->IsOverlap())
		{
			// ���� �ε�ģ ������ ���ٰ� ������� �Լ��� �־���� ��
			return;
		}

		m_IsOverlap = true;
		m_OtherObjType = jellyPushScript->GetJellyPushType();
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

void CJellyPushScript::SetDestinationMove(CGameObject* _Destination)
{
	m_Destination = _Destination;
	m_OtherPos = m_Destination->Transform()->GetRelativePos();
	GetOwner()->AddComponent(new CCountDownDeleteScript);
	CScript* script = GetOwner()->GetScript("CCountDownDeleteScript");
	CCountDownDeleteScript* countDown = dynamic_cast<CCountDownDeleteScript*>(script);
	//countDown->SetSaveTime(TIME);
	countDown->SetDeadTime(0.3f);
}

void CJellyPushScript::DestinationMove()
{
	if (m_Destination == nullptr)
	{
		return;
	}

	Vec3 myPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 pos;
	// ���� ���� ���
	Vec3 direction = m_OtherPos - myPos;
	// ���� ���͸� ����ȭ
	direction.Normalize();

	myPos.x += direction.x * m_Speed * DT;
	myPos.y += direction.y * m_Speed * DT;
	pos = myPos;
	GetOwner()->Transform()->SetRelativePos(pos);
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
