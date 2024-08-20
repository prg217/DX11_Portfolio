#include "pch.h"
#include "TileMapEditor.h"

TileMapEditor::TileMapEditor()
{
}

TileMapEditor::~TileMapEditor()
{
}

void TileMapEditor::Init()
{
}

void TileMapEditor::Update()
{
	/*
	Matrix matTranslation = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	// 크기, 회전, 이동
	Matrix matObjectScaleInv = XMMatrixIdentity();

	if (m_IndependentScale)
	{
		Vec3 vObjectScale = GetOwner()->Transform()->GetWorldScale();
		matObjectScaleInv = XMMatrixScaling(vObjectScale.x, vObjectScale.y, vObjectScale.z);
		matObjectScaleInv = XMMatrixInverse(nullptr, matObjectScaleInv);
	}

	m_matColWorld = matScale * matTranslation * matObjectScaleInv * GetOwner()->Transform()->GetWorldMat();

	// Debug 렌더링으로 충돌체의 위치를 표시
	DrawDebugRect(m_matColWorld, Vec4(1.f, 0.f, 0.f, 1.f), 0.f, false);
	*/
}

void TileMapEditor::Activate()
{
}

void TileMapEditor::Deactivate()
{
}

