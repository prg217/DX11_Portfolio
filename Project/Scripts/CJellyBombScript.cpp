#include "pch.h"
#include "CJellyBombScript.h"

#include "CSwingObjScript.h"
#include "CJellyBombDetectScript.h"
#include "CAniFinishDestroyScript.h"

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
	// 플레이어가 채 휘두르기 중일 때 채 휘두르기 콜라이더랑 만나면 폭발
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
	Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_88_JellyBomb_Activation.wav");
	pSound->Play(1, 1.f, false);

	CreateLight(80.f, Vec3(0.62f, 0.99f, 0.97f));
	CreateLight(0.f, Vec3(0.52f, 0.69f, 0.5f));

	m_IsBomb = true;
	m_SaveTime = TIME;
}

void CJellyBombScript::Bomb()
{
	// 빛이 작았다가 커짐
	// 좀 줄었다가 다시 커졌다가... 커져서
	if (TIME - m_SaveTime >= 3.f)
	{
		Ptr<CSound> pSound = CAssetMgr::GetInst()->FindAsset<CSound>(L"sound\\SFX_89_ObjectExplosion.wav");
		pSound->Play(1, 1.f, false);

		// 익스플로전 이펙트 소환, 폭발, 오브젝트 삭제
		DetectDestroy();

		CreateExplosion();

		DeleteObject(GetOwner());
		m_IsBomb = false;
	}
	else if (TIME - m_SaveTime >= 2.8f)
	{
		m_RadiusLight->Light2D()->SetRadius(60.f);
	}
	else if (TIME - m_SaveTime >= 2.6f)
	{
		m_RadiusLight->Light2D()->SetRadius(35.f);
	}
	else if (TIME - m_SaveTime >= 2.4f)
	{
		m_RadiusLight->Light2D()->SetRadius(60.f);
	}
	else if (TIME - m_SaveTime >= 2.2f)
	{
		m_RadiusLight->Light2D()->SetRadius(35.f);
	}
	else if (TIME - m_SaveTime >= 2.f)
	{
		m_RadiusLight->Light2D()->SetLightColor(Vec3(3.f, 5.f, 3.f));
		m_RadiusLight->Light2D()->SetRadius(60.f);
	}
	else if (TIME - m_SaveTime >= 1.8f)
	{
		m_RadiusLight->Light2D()->SetRadius(35.f);
	}
	else if (TIME - m_SaveTime >= 1.5f)
	{
		m_RadiusLight->Light2D()->SetRadius(50.f);
	}
	else if (TIME - m_SaveTime >= 1.2f)
	{
		m_RadiusLight->Light2D()->SetRadius(35.f);
	}
	else if (TIME - m_SaveTime >= 0.8f)
	{
		m_RadiusLight->Light2D()->SetRadius(50.f);
	}
	else if (TIME - m_SaveTime >= 0.4f)
	{
		m_RadiusLight->Light2D()->SetRadius(35.f);
	}
}

void CJellyBombScript::CreateLight(float _Radius, Vec3 _Color)
{
	// 포인트 라이트 추가
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
	// 자식 오브젝트로 Detect오브젝트를 만들고 Detect의 콜라이더에 걸리는 9번 레이어(파괴 블록) 오브젝트들 삭제
	CGameObject* pDetect = new CGameObject;
	pDetect->SetName(L"JellyBomb_Detect");
	pDetect->AddComponent(new CTransform);
	pDetect->AddComponent(new CCollider2D); 
	pDetect->AddComponent(new CJellyBombDetectScript);

	pDetect->Transform()->SetRelativePos(Vec3(0, 0, 0));
	pDetect->Transform()->SetRelativeScale(Vec3(1, 1, 0));
	pDetect->Collider2D()->SetOffset(Vec3(0, 0, 0));
	pDetect->Collider2D()->SetScale(Vec3(2, 2, 0));

	CreateObject(pDetect, 8);
	AddChildObject(GetOwner(), pDetect);

}

void CJellyBombScript::CreateExplosion()
{
	CGameObject* pExplosion = new CGameObject;
	pExplosion->SetName(L"JellyBomb_Explosion0");
	pExplosion->AddComponent(new CTransform);
	pExplosion->AddComponent(new CFlipBookComponent);
	pExplosion->AddComponent(new CMeshRender);
	pExplosion->AddComponent(new CAniFinishDestroyScript);

	pExplosion->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	pExplosion->Transform()->SetRelativeScale(Vec3(500, 500, 0));

	Ptr<CFlipBook> pFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obstacle\\jellyBomb_explosion_0.flip");
	pExplosion->FlipBookComponent()->AddFlipBook(0, pFlip);
	pExplosion->FlipBookComponent()->Play(0, 5, false);

	Ptr<CMaterial> pAlphaBlendMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	pExplosion->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pExplosion->MeshRender()->SetMaterial(pAlphaBlendMtrl);

	CreateObject(pExplosion, 0);

	pExplosion = new CGameObject;
	pExplosion->SetName(L"JellyBomb_Explosion1");
	pExplosion->AddComponent(new CTransform);
	pExplosion->AddComponent(new CFlipBookComponent);
	pExplosion->AddComponent(new CMeshRender);
	pExplosion->AddComponent(new CAniFinishDestroyScript);

	pExplosion->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
	pExplosion->Transform()->SetRelativeScale(Vec3(500, 500, 0));

	pFlip = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\Obstacle\\jellyBomb_explosion_1.flip");
	pExplosion->FlipBookComponent()->AddFlipBook(0, pFlip);
	pExplosion->FlipBookComponent()->Play(0, 5, false);

	pExplosion->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pExplosion->MeshRender()->SetMaterial(pAlphaBlendMtrl);

	CreateObject(pExplosion, 0);
}
