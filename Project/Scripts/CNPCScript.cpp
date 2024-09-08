#include "pch.h"
#include "CNPCScript.h"

#include "CTextBoxScript.h"
#include "CUIScript.h"

CNPCScript::CNPCScript()
	: CScript(UINT(SCRIPT_TYPE::NPCSCRIPT))
{
}

CNPCScript::CNPCScript(const CNPCScript& _Origin)
	: CScript(_Origin)
{
}

CNPCScript::~CNPCScript()
{
}

void CNPCScript::Begin()
{
}

void CNPCScript::Tick()
{
}

void CNPCScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CNPCScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (GetOwner()->GetUIActive())
	{
		return;
	}

	// 플레이어 상호작용과 오버랩인데 S키가 눌렸다면 대화창
	if (_OtherObject->GetLayerIdx() == 5)
	{
		if (KEY_TAP(KEY::S))
		{
			Ptr<CSprite> pSprite;
			Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");

			CGameObject* pTextBox = new CGameObject;
			pTextBox->SetName(L"TextBox");
			pTextBox->AddComponent(new CTransform);
			pTextBox->AddComponent(new CMeshRender);
			pTextBox->AddComponent(new CSpriteComponent);
			pTextBox->AddComponent(new CUIScript);
			pTextBox->AddComponent(new CTextBoxScript);

			pTextBox->Transform()->SetRelativePos(0.f, -230.0f, 0.f);
			pTextBox->Transform()->SetRelativeScale(900.f, 300.f, 0.f);

			pTextBox->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			pTextBox->MeshRender()->SetMaterial(pMtrl);

			pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\ui\\textBox_main.sprite");
			pTextBox->SpriteComponent()->AddSprite(pSprite);
			pTextBox->SpriteComponent()->AddColor(true, Vec3(0.45f, 0.38f, 0.32f));
			pTextBox->SpriteComponent()->SetUseLight(false);

			CScript* script = pTextBox->GetScript("CTextBoxScript");
			CTextBoxScript* texScript = dynamic_cast<CTextBoxScript*>(script);
			texScript->SetText(L"헤롱 헤롱...\n뭔 일 있어?");
			texScript->SetText(L"므어어엉");

			CreateObject(pTextBox, 31);

			// 이름표도 만들어서 붙여줌
			CGameObject* pTextBoxName = new CGameObject;
			pTextBoxName->SetName(L"TextBoxName");
			pTextBoxName->AddComponent(new CTransform);
			pTextBoxName->AddComponent(new CMeshRender);
			pTextBoxName->AddComponent(new CSpriteComponent);
			pTextBoxName->AddComponent(new CUIScript);
			pTextBoxName->AddComponent(new CTextBoxScript);

			pTextBoxName->Transform()->SetRelativePos(-0.35f, 0.35f, 0.f);
			pTextBoxName->Transform()->SetRelativeScale(0.3f, 1.1f, 0.f);

			pTextBoxName->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			pTextBoxName->MeshRender()->SetMaterial(pMtrl);

			pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\ui\\textBox_name.sprite");
			pTextBoxName->SpriteComponent()->AddSprite(pSprite);
			pTextBoxName->SpriteComponent()->AddColor(true, Vec3(0.45f, 0.38f, 0.32f));
			pTextBoxName->SpriteComponent()->SetUseLight(false);

			script = pTextBoxName->GetScript("CTextBoxScript");
			texScript = dynamic_cast<CTextBoxScript*>(script);
			texScript->IsName();
			texScript->SetText(L"벌레 주민");

			CreateObject(pTextBoxName, 31);
			AddChildObject(pTextBox, pTextBoxName);
		}
	}
}

void CNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CNPCScript::SaveToFile(FILE* _File)
{
}

void CNPCScript::LoadFromFile(FILE* _File)
{
}
