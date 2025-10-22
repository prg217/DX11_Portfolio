#include "pch.h"
#include "CSpriteComponent.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CAssetMgr.h"

CSpriteComponent::CSpriteComponent()
	: CComponent(COMPONENT_TYPE::SPRITECOMPONENT)
	, m_Sprite(nullptr)
	, m_Outline(false)
	, m_AddColor(false)
	, m_Color(Vec3(0.f, 0.f, 0.f))
	, m_Alpha(1.f)
	, m_UseLight(true)
	, m_UseSliceAmend(false)
{
}	 

CSpriteComponent::CSpriteComponent(CSpriteComponent& _Origin)
	: CComponent(_Origin)
	, m_Sprite(_Origin.m_Sprite)
	, m_Outline(_Origin.m_Outline)
	, m_AddColor(_Origin.m_AddColor)
	, m_Color(_Origin.m_Color)
	, m_Alpha(1.f)
	, m_UseLight(_Origin.m_Color)
	, m_UseSliceAmend(false)
{
}

CSpriteComponent::~CSpriteComponent()
{
}

void CSpriteComponent::FinalTick()
{
}

void CSpriteComponent::AddSprite(Ptr<CSprite> _Sprtie)
{
	m_Sprite = _Sprtie;
}

void CSpriteComponent::Binding()
{
	if (nullptr != m_Sprite)
	{
		// 현재 표시해야하는 Sprite 의 정보를 Sprite 전용 상수버퍼를 통해서 GPU 에 전달
		tSpriteInfo tInfo = {};

		tInfo.LeftTopUV = m_Sprite->GetLeftTopUV();
		if (m_UseSliceAmend)
		{
			tInfo.SliceUV = m_SliceAmend;
		}
		else
		{
			tInfo.SliceUV = m_Sprite->GetSliceUV();
		}
		tInfo.BackGroundUV = m_Sprite->GetBackgroundUV();
		tInfo.OffsetUV = m_Sprite->GetOffsetUV();
		tInfo.TexSize = m_Sprite->GetTexSize();
		tInfo.UseFlipbook = 0;
		tInfo.UseOutline = m_Outline;
		tInfo.UseAddColor = m_AddColor;
		tInfo.AddColor = m_Color;
		tInfo.AddAlpha = m_Alpha;
		tInfo.UseLight = m_UseLight;

		static CConstBuffer* CB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);

		CB->SetData(&tInfo);
		CB->Binding();

		// FlipBook Sprite 아틀라스 텍스쳐 전용 레지스터번호 t10 에 바인딩
		Ptr<CTexture> pAtlas = m_Sprite->GetAtlasTexture();
		pAtlas->Binding(10);
	}
}

void CSpriteComponent::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_Sprite, _File);
	// 외곽선 유무
	fwrite(&m_Outline, sizeof(bool), 1, _File);
	// 색 추가 유무 및 색
	fwrite(&m_AddColor, sizeof(bool), 1, _File);
	fwrite(&m_Color, sizeof(Vec3), 1, _File);
	fwrite(&m_Alpha, sizeof(float), 1, _File);
	// 빛 유무
	fwrite(&m_UseLight, sizeof(bool), 1, _File);
}

void CSpriteComponent::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_Sprite, _File);
	// 아웃라이너 유무
	fread(&m_Outline, sizeof(bool), 1, _File);
	// 색 추가 유무 및 색
	fread(&m_AddColor, sizeof(bool), 1, _File);
	fread(&m_Color, sizeof(Vec3), 1, _File);
	fread(&m_Alpha, sizeof(float), 1, _File);
	// 빛 유무
	fread(&m_UseLight, sizeof(bool), 1, _File);
}

void CSpriteComponent::AddColor(bool _AddColor, Vec3 Color)
{
	m_AddColor = _AddColor;
	m_Color = Color;
}

void CSpriteComponent::SliceAmend(bool _Use, Vec2 _Slice)
{
	m_UseSliceAmend = _Use;
	m_SliceAmend = _Slice / m_Sprite->GetTexSize();
}

void CSpriteComponent::SliceAmendUV(bool _Use, Vec2 _SliceUV)
{
	m_UseSliceAmend = _Use;
	m_SliceAmend = _SliceUV;
}
