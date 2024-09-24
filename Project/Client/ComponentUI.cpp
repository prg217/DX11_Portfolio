#include "pch.h"
#include "ComponentUI.h"

#include "Inspector.h"
#include "CEditorMgr.h"

#include <Engine/CGameObject.h>
#include <Engine/CComponent.h>

ComponentUI::ComponentUI(COMPONENT_TYPE _Type)
	: m_TargetObject(nullptr)
	, m_Type(_Type)
{
	SetChildBorder(true);
}

ComponentUI::~ComponentUI()
{
}


void ComponentUI::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	if (m_TargetObject == nullptr)
	{
		SetActive(false);
		return;
	}

	if (m_TargetObject->IsDead())
	{
		SetActive(false);
		return;
	}

	if (nullptr != m_TargetObject && 
		nullptr != m_TargetObject->GetComponent(m_Type))
	{
		SetActive(true);
	}

	else
	{
		SetActive(false);
	}
}

void ComponentUI::Title()
{
	ImGui::PushID((int)m_Type);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
		
	ImGui::Button(ToString(m_Type));
	// ��ư�� ������ Ŭ���Ǿ��� �� �˾� ����
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("ComponentUIOption");
	}
	// �˾��� ������ �� ǥ���� ����
	if (ImGui::BeginPopup("ComponentUIOption"))
	{
		ImGui::Button("DeleteComponent");
		{
			// �ν�����
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

			// Ÿ�� ������Ʈ �˾Ƴ�
			CGameObject* pObject = pInspector->GetTargetObject();
			if (nullptr != pObject)
			{
				pObject->ObjDeleteComponent(m_Type);
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::Button("Close");
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}