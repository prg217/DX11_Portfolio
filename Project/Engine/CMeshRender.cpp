#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CFlipBookComponent.h"
#include "CSprite.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{
	if (!GetMesh() || !GetMaterial() || !GetMaterial()->GetShader())
		return;
	
	// FlipBookComponent �� ������ ���� ������� Sprite ������ Binding �ϰ� �Ѵ�.
	if (FlipBookComponent())
		FlipBookComponent()->Binding();	
	else
		CFlipBookComponent::Clear();	

	// ��ġ, ũ��, ȸ�� �������� ���ε�
	Transform()->Binding();

	// ���� ���ε�(���� ���, ���̴� ���)
	GetMaterial()->Binding();

	// ���ؽ�����, �ε������� ���ε� �� ������ ȣ��
	GetMesh()->Render();
}

void CMeshRender::SaveToFile(FILE* _File)
{
	SaveDataToFile(_File);
}

void CMeshRender::LoadFromFile(FILE* _File)
{
	LoadDataFromFile(_File);
}
