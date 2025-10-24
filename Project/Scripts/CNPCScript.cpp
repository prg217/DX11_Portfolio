#include "pch.h"
#include "CNPCScript.h"

#include "CTextBoxScript.h"
#include "CUIScript.h"

CNPCScript::CNPCScript()
	: CScript(UINT(SCRIPT_TYPE::NPCSCRIPT))
	, m_pTextBox(nullptr)
	, m_textSize(0)
{
}

CNPCScript::CNPCScript(const CNPCScript& _Origin)
	: CScript(_Origin)
	, m_pTextBox(nullptr)
	, m_textSize(0)
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
			CreateTextBox();
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

void CNPCScript::CreateTextBox()
{
	Ptr<CSprite> pSprite;
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");

	m_pTextBox = new CGameObject;
	m_pTextBox->SetName(L"TextBox");
	m_pTextBox->AddComponent(new CTransform);
	m_pTextBox->AddComponent(new CMeshRender);
	m_pTextBox->AddComponent(new CSpriteComponent);
	m_pTextBox->AddComponent(new CUIScript);
	m_pTextBox->AddComponent(new CTextBoxScript);

	m_pTextBox->Transform()->SetRelativePos(0.f, -230.0f, 0.f);
	m_pTextBox->Transform()->SetRelativeScale(900.f, 300.f, 0.f);

	m_pTextBox->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	m_pTextBox->MeshRender()->SetMaterial(pMtrl);

	pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(L"sprite\\ui\\textBox_main.sprite");
	m_pTextBox->SpriteComponent()->AddSprite(pSprite);
	m_pTextBox->SpriteComponent()->AddColor(true, Vec3(0.45f, 0.38f, 0.32f));
	m_pTextBox->SpriteComponent()->SetUseLight(false);

	CScript* script = m_pTextBox->GetScript("CTextBoxScript");
	CTextBoxScript* texScript = dynamic_cast<CTextBoxScript*>(script);

	if (m_vText.size() == 0)
	{
		texScript->SetText(L"텍스트 없음");
	}
	for (auto i : m_vText)
	{
		texScript->SetText(i);
	}

	CreateObject(m_pTextBox, 31);

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
	texScript->SetText(m_NPCName);

	CreateObject(pTextBoxName, 31);
	AddChildObject(m_pTextBox, pTextBoxName);
}

void CNPCScript::SetName(wstring _Text)
{
	m_NPCName = _Text;
}

void CNPCScript::SetText(wstring _Text)
{
	m_vText.push_back(_Text);
}

void CNPCScript::LoadText(const wstring& _FileName)
{
	// 임시로 받을 string 텍스트
	vector<string> stringText;

	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"textBox\\";
	strInitPath += _FileName;
	strInitPath += L".textBox";

	FILE* File = nullptr;
	_wfopen_s(&File, strInitPath.c_str(), L"rb");

	if (File == nullptr)
	{
		return;
	}

	fread(&m_textSize, sizeof(int), 1, File);
	stringText.clear();
	stringText.reserve(m_textSize);

	for (int i = 0; i < m_textSize; ++i)
	{
		size_t strLen = 0;
		fread(&strLen, sizeof(size_t), 1, File);

		if (strLen > 0)
		{
			vector<char> buffer(strLen + 1, 0);
			fread(buffer.data(), sizeof(char), strLen, File);
			stringText.push_back(string(buffer.data()));
		}
		// 내용이 비었으면
		else
		{
			stringText.push_back("");
		}

		fclose(File);

		// 임시로 받은 string텍스트 wstring변환
		if (stringText.empty())
		{
			return;
		}

		m_vText.clear();
		m_vText.reserve(m_textSize);

		for (int i = 0; i < m_textSize; ++i)
		{
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, stringText[i].c_str(), (int)stringText[i].size(), NULL, 0);
			std::wstring wstr(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, stringText[i].c_str(), (int)stringText[i].size(), &wstr[0], size_needed);

			m_vText.push_back(wstr);
		}
	}
}