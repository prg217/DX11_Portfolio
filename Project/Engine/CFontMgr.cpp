#include "pch.h"
#include "CFontMgr.h"

#include "CDevice.h"

CFontMgr::CFontMgr()
	: m_FW1Factory(nullptr)
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	for (const auto& pair : m_FontWrappers)
	{
		pair.second->Release();
	}
}

void CFontMgr::Init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(nullptr);
	}

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"카페24 써라운드 에어", &m_FontWrappers[L"카페24 써라운드 에어"])))
	{
		assert(nullptr);
	}
	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"카페24 써라운드", &m_FontWrappers[L"카페24 써라운드"])))
	{
		assert(nullptr);
	}
}

void CFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, wstring _FontName)
{
	m_FontWrappers[_FontName]->DrawString(
		CONTEXT,
		_pStr,				// String
		_fFontSize,			// Font size
		_fPosX,				// X position
		_fPosY,				// Y position
		_Color,				// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE    // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawCenterFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, wstring _FontName)
{
	m_FontWrappers[_FontName]->DrawString(
		CONTEXT,
		_pStr,				// String
		_fFontSize,			// Font size
		_fPosX,				// X position
		_fPosY,				// Y position
		_Color,				// Text color, 0xAaBbGgRr
		FW1_CENTER    // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
