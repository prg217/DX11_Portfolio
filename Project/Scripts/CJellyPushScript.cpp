#include "pch.h"
#include "CJellyPushScript.h"

#include "CInteractionScript.h"
#include "CPushScript.h"
#include "CCountDownDeleteScript.h"
#include "CLiftScript.h"

CJellyPushScript::CJellyPushScript()
	: CScript(UINT(SCRIPT_TYPE::JELLYPUSHSCRIPT))
	, m_Type(JellyPushType::NONE)
	, m_OtherObjType(JellyPushType::NONE)
	, m_IsOverlap(false)
	, m_Destination(nullptr)
	, m_Speed(100.f)
	, m_SaveSpawnTime(TIME)
	, m_Shaking(false)
	, m_SaveShakingTime(0)
{
	AddScriptParam(SCRIPT_PARAM::INT, "JellyPushType", &m_Type);
	AddScriptParam(SCRIPT_PARAM::INT, "Speed", &m_Speed);
}

CJellyPushScript::CJellyPushScript(const CJellyPushScript& _Origin)
	: CScript(_Origin)
	, m_Type(JellyPushType::NONE)
	, m_OtherObjType(JellyPushType::NONE)
	, m_IsOverlap(false)
	, m_Destination(nullptr)
	, m_Speed(100.f)
	, m_SaveSpawnTime(TIME)
	, m_Shaking(false)
	, m_SaveShakingTime(0)
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
	if (m_Shaking)
	{
		Shaking();
	}

	DestinationMove();

	if (GetOwner()->IsDead())
	{
		switch (m_OtherObjType)
		{
		case JellyPushType::CYAN:
			switch (m_Type)
			{
			case JellyPushType::MAGENTA:
				CreateBigJellyPush(JellyPushType::BLUE);
				break;
			case JellyPushType::YELLOW:
				CreateBigJellyPush(JellyPushType::GREEN);
				break;
			default:
				break;
			}
			break;
		case JellyPushType::MAGENTA:
			switch (m_Type)
			{
			case JellyPushType::CYAN:
				CreateBigJellyPush(JellyPushType::BLUE);
				break;
			case JellyPushType::YELLOW:
				CreateBigJellyPush(JellyPushType::RED);
				break;
			default:
				break;
			}
			break;
		case JellyPushType::YELLOW:
			switch (m_Type)
			{
			case JellyPushType::CYAN:
				CreateBigJellyPush(JellyPushType::GREEN);
				break;
			case JellyPushType::MAGENTA:
				CreateBigJellyPush(JellyPushType::RED);
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
	// �÷��̾�� ������� ��鸰��.
	if (_OtherObject->GetLayerIdx() == 3)
	{
		m_Shaking = true;
	}

	// �θ� ������ return
	if (GetOwner()->GetParent() != nullptr)
	{
		return;
	}
	if (_OtherObject->GetParent() != nullptr)
	{
		return;
	}

	// �������� �� �� ������ return
	if (TIME - m_SaveSpawnTime < 1.f)
	{
		return;
	}

	// JellyPushFrame�� ������� �ڵ����� ����� ����
	if (_OtherObject->GetScript("CJellyPushFrameScript") != nullptr)
	{
		// _OtherObject�� ���� �̵�
		m_Speed = 3000.f;
		SetDestinationMove(_OtherObject, false);
	}

	// �̴� ���� �� ���
	// ���� ���̸� ���� �з���
	// �ٸ� ���̸� �� ���� ���� �ռ���(���� ��ġ ��� �������� ���ٰ� ������)
	switch (m_Type)
	{
	case JellyPushType::CYAN:
	case JellyPushType::MAGENTA:
	case JellyPushType::YELLOW:
	{
		CScript* script = _OtherObject->GetScript("CJellyPushScript");
		CJellyPushScript* jellyPushScript = dynamic_cast<CJellyPushScript*>(script);

		if (jellyPushScript != nullptr)
		{
			// �ڽ��� �̹� �������� �����ߴٸ� return
			if (m_IsOverlap)
			{
				return;
			}

			switch (jellyPushScript->GetJellyPushType())
			{
			case JellyPushType::CYAN:
			case JellyPushType::MAGENTA:
			case JellyPushType::YELLOW:
			{
				// _OtherObject�� ���� �̵�
				m_Speed = 100.f;
				SetDestinationMove(_OtherObject, true);

				// ������ �̹� ������ �ڵ带 �����ϰ� ������ return
				if (jellyPushScript->IsOverlap())
				{
					// ���� �ε�ģ ������ ���ٰ� ������� �Լ��� �־���� ��
					return;
				}

				m_IsOverlap = true;
				m_OtherObjType = jellyPushScript->GetJellyPushType();
			}
				break;
			default:
				break;
			}
		}
	}
		break;
	default:
		break;
	}
}

void CJellyPushScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyPushScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// JellyPushFrame���� �����
	if (_OtherObject->GetScript("CJellyPushFrameScript") != nullptr)
	{
		// �̵� ����
		m_Speed = 100.f;
		m_Destination = nullptr;
	}
}

void CJellyPushScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Type, sizeof(JellyPushType), 1, _File);
}

void CJellyPushScript::LoadFromFile(FILE* _File)
{
	fread(&m_Type, sizeof(JellyPushType), 1, _File);
}

void CJellyPushScript::SetDestinationMove(CGameObject* _Destination, bool _Dead)
{
	m_Destination = _Destination;
	m_OtherPos = m_Destination->Transform()->GetRelativePos();
	if (_Dead)
	{
		GetOwner()->AddComponent(new CCountDownDeleteScript);
		CScript* script = GetOwner()->GetScript("CCountDownDeleteScript");
		CCountDownDeleteScript* countDown = dynamic_cast<CCountDownDeleteScript*>(script);

		countDown->SetDeadTime(0.3f);
	}
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

void CJellyPushScript::CreateBigJellyPush(JellyPushType _Type)
{
	// �ռ� �� �� ��ƼŬ 3��
	CreateParticle();

	CGameObject* jelly = new CGameObject;

	jelly->AddComponent(new CTransform);
	jelly->AddComponent(new CCollider2D);
	jelly->AddComponent(new CMeshRender);
	jelly->AddComponent(new CSpriteComponent);
	jelly->AddComponent(new CRigidBody);
	jelly->AddComponent(new CInteractionScript);
	jelly->AddComponent(new CJellyPushScript);
	jelly->AddComponent(new CPushScript);

	jelly->Transform()->SetIndependentScale(true);
	jelly->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	jelly->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 0.f));

	jelly->Collider2D()->SetIndependentScale(false);
	jelly->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	jelly->Collider2D()->SetScale(Vec3(0.4f, 0.2f, 1.f));

	jelly->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	jelly->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	CScript* script = jelly->GetScript("CJellyPushScript");
	CJellyPushScript* jellyScript = dynamic_cast<CJellyPushScript*>(script);

	Ptr<CSprite> pSprite;
	switch (_Type)
	{
	case JellyPushType::BLUE:
		jelly->SetName(L"JellyPush_Blue");
		pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\obstacle\\jellyPush_blue.sprite");
		jellyScript->SetType(JellyPushType::BLUE);
		break;
	case JellyPushType::GREEN:
		jelly->SetName(L"JellyPush_Green");
		pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\obstacle\\jellyPush_green.sprite");
		jellyScript->SetType(JellyPushType::GREEN);
		break;
	case JellyPushType::RED:
		jelly->SetName(L"JellyPush_Red");
		pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\obstacle\\jellyPush_red.sprite");
		jellyScript->SetType(JellyPushType::RED);
		break;
	default:
		break;
	}

	jelly->SpriteComponent()->AddSprite(pSprite);

	CreateObject(jelly, 6);
}

CGameObject* CJellyPushScript::CreateMiniJellyPush(JellyPushType _Type)
{
	CGameObject* jelly;

	jelly = new CGameObject;
	jelly->AddComponent(new CTransform);
	jelly->AddComponent(new CCollider2D);
	jelly->AddComponent(new CMeshRender);
	jelly->AddComponent(new CSpriteComponent);
	jelly->AddComponent(new CRigidBody);
	jelly->AddComponent(new CInteractionScript);
	jelly->AddComponent(new CJellyPushScript);
	jelly->AddComponent(new CPushScript);
	jelly->AddComponent(new CLiftScript);

	jelly->Transform()->SetIndependentScale(true);
	jelly->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	jelly->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 0.f));

	jelly->Collider2D()->SetIndependentScale(false);
	jelly->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	jelly->Collider2D()->SetScale(Vec3(0.25f, 0.15f, 1.f));

	jelly->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	jelly->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	CScript* script = jelly->GetScript("CJellyPushScript");
	CJellyPushScript* jellyScript = dynamic_cast<CJellyPushScript*>(script);

	Ptr<CSprite> pSprite;
	switch (_Type)
	{
	case JellyPushType::CYAN:
		jelly->SetName(L"JellyPush_Cyan");
		pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\obstacle\\jellyPush_cyan.sprite");
		jellyScript->SetType(JellyPushType::CYAN);
		break;
	case JellyPushType::MAGENTA:
		jelly->SetName(L"JellyPush_Magenta");
		pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\obstacle\\jellyPush_magenta.sprite");
		jellyScript->SetType(JellyPushType::MAGENTA);
		break;
	case JellyPushType::YELLOW:
		jelly->SetName(L"JellyPush_Yellow");
		pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\obstacle\\jellyPush_yellow.sprite");
		jellyScript->SetType(JellyPushType::YELLOW);
		break;
	default:
		break;
	}
	jelly->SpriteComponent()->AddSprite(pSprite);

	CreateObject(jelly, 6);

	return jelly;
}

void CJellyPushScript::CreateParticle()
{
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"JellyPushParticle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"particle\\jellyPush_0.particle";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	pParticleObj->ParticleSystem()->LoadFromFile(File);
	fclose(File);

	// �����Ǵ� �ð��� �������ش�.
	pParticleObj->AddComponent(new CCountDownDeleteScript);
	CScript* pScript = pParticleObj->GetScript("CCountDownDeleteScript");
	CCountDownDeleteScript* pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
	pCountDown->SetSaveTime(TIME);
	pCountDown->SetDeadTime(1.5f);

	pParticleObj->Transform()->SetRelativePos(Transform()->GetRelativePos());

	CreateObject(pParticleObj, 0);

	pParticleObj = new CGameObject;
	pParticleObj->SetName(L"JellyPushParticle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"particle\\jellyPush_1.particle";

	File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	pParticleObj->ParticleSystem()->LoadFromFile(File);
	fclose(File);

	// �����Ǵ� �ð��� �������ش�.
	pParticleObj->AddComponent(new CCountDownDeleteScript);
	pScript = pParticleObj->GetScript("CCountDownDeleteScript");
	pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
	pCountDown->SetSaveTime(TIME);
	pCountDown->SetDeadTime(1.5f);

	pParticleObj->Transform()->SetRelativePos(Transform()->GetRelativePos());

	CreateObject(pParticleObj, 0);

	pParticleObj = new CGameObject;
	pParticleObj->SetName(L"JellyPushParticle");

	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"particle\\jellyPush_2.particle";

	File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	pParticleObj->ParticleSystem()->LoadFromFile(File);
	fclose(File);

	// �����Ǵ� �ð��� �������ش�.
	pParticleObj->AddComponent(new CCountDownDeleteScript);
	pScript = pParticleObj->GetScript("CCountDownDeleteScript");
	pCountDown = dynamic_cast<CCountDownDeleteScript*>(pScript);
	pCountDown->SetSaveTime(TIME);
	pCountDown->SetDeadTime(1.5f);

	pParticleObj->Transform()->SetRelativePos(Transform()->GetRelativePos());

	CreateObject(pParticleObj, 0);
}

void CJellyPushScript::Shaking()
{
	if (m_SaveShakingTime == 0.f)
	{
		m_SaveShakingTime = TIME;
	}
	Vec3 scale = GetOwner()->Transform()->GetRelativeScale();

	float shaking = 3.f;
	float amplitude = 5.f; // ����
	float x = 150.f + amplitude * sin(shaking * 2.0f * 3.14159f * TIME);
	float y = 150.f + amplitude * sin(-shaking * 2.0f * 3.14159f * TIME);
	GetOwner()->Transform()->SetRelativeScale(Vec3(x, y, 0));

	if (TIME - m_SaveShakingTime >= 0.6f)
	{
		m_Shaking = false;
		m_SaveShakingTime = 0.f;
	}
}

CGameObject* CJellyPushScript::Speparation()
{
	DeleteObject(GetOwner());

	// �ϳ��� �״�� ���� �ϳ��� Lift���·� �ٷ� ������.
	switch (m_Type)
	{
	case JellyPushType::BLUE:
		CreateMiniJellyPush(JellyPushType::CYAN);
		return CreateMiniJellyPush(JellyPushType::MAGENTA);
		break;
	case JellyPushType::GREEN:
		CreateMiniJellyPush(JellyPushType::YELLOW);	
		return CreateMiniJellyPush(JellyPushType::CYAN);
		break;
	case JellyPushType::RED:
		CreateMiniJellyPush(JellyPushType::MAGENTA);
		return CreateMiniJellyPush(JellyPushType::YELLOW);
		break;
	default:
		break;
	}

	return nullptr;
}
