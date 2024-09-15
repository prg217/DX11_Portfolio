#include "pch.h"
#include "CJellyBombScript.h"

#include "CSwingObjScript.h"

CJellyBombScript::CJellyBombScript()
	 : CScript(UINT(SCRIPT_TYPE::JELLYBOMBSCRIPT))
	, m_IsBomb(false)
	, m_SaveTime(0)
	, m_RadiusLight(nullptr)
{
}

CJellyBombScript::CJellyBombScript(const CJellyBombScript& _Origin)
	: CScript(_Origin)
	, m_IsBomb(false)
	, m_SaveTime(0)
	, m_RadiusLight(nullptr)
{
}

CJellyBombScript::~CJellyBombScript()
{
}

void CJellyBombScript::Begin()
{
}

void CJellyBombScript::Tick()
{
	if (m_IsBomb)
	{
		Bomb();
	}
}

void CJellyBombScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyBombScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	// �÷��̾ ä �ֵθ��� ���� �� ä �ֵθ��� �ݶ��̴��� ������ ����
	if (_OtherObject->GetLayerIdx() == 7)
	{
		CScript* script = _OtherObject->GetScript("CSwingObjScript");
		CSwingObjScript* pSwing = dynamic_cast<CSwingObjScript*>(script);
		if (pSwing)
		{
			if (pSwing->GetIsSwing() && !m_IsBomb)
			{
				BombStart();
			}
		}
	}
}

void CJellyBombScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CJellyBombScript::SaveToFile(FILE* _File)
{
}

void CJellyBombScript::LoadFromFile(FILE* _File)
{
}

void CJellyBombScript::BombStart()
{
	CreateLight(80.f, Vec3(0.62f, 0.99f, 0.97f));
	CreateLight(0.f, Vec3(0.52f, 0.69f, 0.5f));

	m_IsBomb = true;
	m_SaveTime = TIME;
}

void CJellyBombScript::Bomb()
{
	// ���� �۾Ҵٰ� Ŀ��
	// �� �پ��ٰ� �ٽ� Ŀ���ٰ�... Ŀ����
	if (TIME - m_SaveTime >= 1.2f)
	{
		// �ͽ��÷��� ����Ʈ ��ȯ, ����, ������Ʈ ����
		DetectDestroy();
		m_IsBomb = false;
	}
	else if (TIME - m_SaveTime >= 1.f)
	{
		m_RadiusLight->Light2D()->SetRadius(60.f);
	}
	else if (TIME - m_SaveTime >= 0.9f)
	{
		m_RadiusLight->Light2D()->SetRadius(35.f);
	}
	else if (TIME - m_SaveTime >= 0.7f)
	{
		m_RadiusLight->Light2D()->SetRadius(50.f);
	}
	else if (TIME - m_SaveTime >= 0.5f)
	{
		m_RadiusLight->Light2D()->SetRadius(35.f);
	}
	else if (TIME - m_SaveTime >= 0.3f)
	{
		m_RadiusLight->Light2D()->SetRadius(50.f);
	}
	else if (TIME - m_SaveTime >= 0.2f)
	{
		m_RadiusLight->Light2D()->SetRadius(35.f);
	}
}

void CJellyBombScript::CreateLight(float _Radius, Vec3 _Color)
{
	// ����Ʈ ����Ʈ �߰�
	CGameObject* pLight = new CGameObject;
	pLight->SetName(L"JellyBomb_Light");
	pLight->AddComponent(new CTransform);
	pLight->AddComponent(new CLight2D);

	pLight->Transform()->SetRelativePos(Vec3(0, 0, 0));
	pLight->Transform()->SetRelativeScale(Vec3(1, 1, 0));

	pLight->Light2D()->SetLightColor(_Color);
	pLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLight->Light2D()->SetRadius(_Radius);

	CreateObject(pLight, 0);
	AddChildObject(GetOwner(), pLight);

	m_RadiusLight = pLight;
}

void CJellyBombScript::DetectDestroy()
{
	// �ڽ� ������Ʈ�� Detect������Ʈ�� ����� Detect�� �ݶ��̴��� �ɸ��� 9�� ���̾�(�ı� ���) ������Ʈ�� ����
	CGameObject* pDetect = new CGameObject;
	pDetect->SetName(L"JellyBomb_Detect");
	pDetect->AddComponent(new CTransform);
	pDetect->AddComponent(new CCollider2D);

	pDetect->Transform()->SetRelativePos(Vec3(0, 0, 0));
	pDetect->Transform()->SetRelativeScale(Vec3(1, 1, 0));
	pDetect->Collider2D()->SetOffset(Vec3(0, 0, 0));
	pDetect->Collider2D()->SetScale(Vec3(2, 2, 0));

	CreateObject(pDetect, 8);
	AddChildObject(GetOwner(), pDetect);

}
