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
			Ptr<CFlipBook> pFlipBook;
			Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");

			CGameObject* pTextBox = new CGameObject;
			pTextBox->SetName(L"TextBox");
			pTextBox->AddComponent(new CTransform);
			pTextBox->AddComponent(new CMeshRender);
			pTextBox->AddComponent(new CFlipBookComponent);
			pTextBox->AddComponent(new CUIScript);
			pTextBox->AddComponent(new CTextBoxScript);

			pTextBox->Transform()->SetRelativePos(0.f, -230.0f, 0.f);
			pTextBox->Transform()->SetRelativeScale(900.f, 300.f, 0.f);

			pTextBox->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			pTextBox->MeshRender()->SetMaterial(pMtrl);

			pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\UI\\textBox_main.flip");
			pTextBox->FlipBookComponent()->AddFlipBook(0, pFlipBook);
			pTextBox->FlipBookComponent()->Play(0, 0, false);

			CScript* script = pTextBox->GetScript("CTextBoxScript");
			CTextBoxScript* texScript = dynamic_cast<CTextBoxScript*>(script);
			texScript->SetText(0, 0, L"헤롱 헤롱...");

			CreateObject(pTextBox, 31);

			// 이름표도 만들어서 붙여줌
			CGameObject* pTextBoxName = new CGameObject;
			pTextBoxName->SetName(L"TextBoxName");
			pTextBoxName->AddComponent(new CTransform);
			pTextBoxName->AddComponent(new CMeshRender);
			pTextBoxName->AddComponent(new CFlipBookComponent);
			pTextBoxName->AddComponent(new CUIScript);
			pTextBoxName->AddComponent(new CTextBoxScript);

			pTextBoxName->Transform()->SetRelativePos(-0.35f, 0.35f, 0.f);
			pTextBoxName->Transform()->SetRelativeScale(0.3f, 1.1f, 0.f);

			pTextBoxName->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
			pTextBoxName->MeshRender()->SetMaterial(pMtrl);

			pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"Animation\\UI\\textBox_name.flip");
			pTextBoxName->FlipBookComponent()->AddFlipBook(0, pFlipBook);
			pTextBoxName->FlipBookComponent()->Play(0, 0, false);

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
