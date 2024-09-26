#pragma once

#include "singleton.h"

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug.lib")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper.lib"))
#endif

#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)


class CFontMgr :
    public CSingleton<CFontMgr>
{
    SINGLE(CFontMgr);
private:
    IFW1Factory*        m_FW1Factory;
    //IFW1FontWrapper*    m_FontWrapper;
    map<wstring, IFW1FontWrapper*> m_FontWrappers; // 폰트 이름을 키로 사용

public:
    void Init();
    void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, wstring _FontName = L"카페24 써라운드 에어");
    void DrawCenterFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color, wstring _FontName = L"카페24 써라운드 에어");
};

