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
	// 버튼이 오른쪽 클릭되었을 때 팝업 열기
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("ComponentUIOption");
	}
	// 팝업이 열렸을 때 표시할 내용
	if (ImGui::BeginPopup("ComponentUIOption"))
	{
		ImGui::Button("DeleteComponent");
		{
			// 인스펙터
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

			// 타겟 오브젝트 알아냄
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