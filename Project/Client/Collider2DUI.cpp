#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>


Collider2DUI::Collider2DUI()
	: ComponentUI(COMPONENT_TYPE::COLLIDER2D)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Update()
{
	Title();

	CCollider2D* pColider2D = GetTargetObject()->Collider2D();

	Vec3 vOffsetPos = pColider2D->GetOffset();
	Vec3 vOffsetScale = pColider2D->GetScale();
	// Offset Scale

	// Offset Pos
	ImGui::Text("Offset Position");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##OffsetPos", vOffsetPos);
	pColider2D->SetOffset(vOffsetPos);

	// Offset Scale
	ImGui::Text("Offset Scale");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##OffsetScale", vOffsetScale);
	pColider2D->SetScale(vOffsetScale);

	// Independent Scale CheckBox
	bool IS = pColider2D->GetIndependentScale();

	ImGui::Text("Ignore Parent");
	ImGui::SameLine(100);
	if (ImGui::Checkbox("##Collider2DIS", &IS))
	{
		pColider2D->SetIndependentScale(IS);
	}

}