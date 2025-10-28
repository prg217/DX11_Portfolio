#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"


CTexture::CTexture()
	: CAsset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
	, m_IsArray(false)
{

}

CTexture::~CTexture()
{
}

void CTexture::Binding(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());	
}

void CTexture::Binding_CS_SRV(UINT _RegisterNum)
{
	m_RecentBindingRegisterNum = _RegisterNum;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Binding_CS_UAV(UINT _RegisterNum)
{
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
	m_RecentBindingRegisterNum = _RegisterNum;
}

void CTexture::Clear(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void CTexture::Clear_CS_SRV()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(m_RecentBindingRegisterNum, 1, &pSRV);
}

void CTexture::Clear_CS_UAV()
{
	UINT i = -1;
	ID3D11UnorderedAccessView* pUAV = nullptr;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentBindingRegisterNum, 1, &pUAV, &i);
}

int CTexture::CreateTextureArray(UINT _Width, UINT _Height, UINT _ArraySize, DXGI_FORMAT _PixelFormat, UINT _Flags, D3D11_USAGE _Usage)
{
	m_Desc.Width = _Width;
	m_Desc.Height = _Height;
	m_Desc.Format = _PixelFormat;
	m_Desc.ArraySize = _ArraySize;
	m_Desc.BindFlags = _Flags;
	m_Desc.Usage = _Usage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.CPUAccessFlags = 0;
	}

	m_Desc.MiscFlags = 0;
	m_Desc.MipLevels = 1;
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}

	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = m_Desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.MipLevels = 1;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.ArraySize = _ArraySize;

		if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &srvDesc, m_SRV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	// RTV, DSV 등도 필요하면 배열용으로 생성
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
	}

	m_IsArray = true;
	return S_OK;
}

int CTexture::CopyToArray(UINT _ArrayIndex, Ptr<CTexture> _SrcTexture)
{
	if (!m_IsArray || !_SrcTexture)
		return E_FAIL;

	if (_ArrayIndex >= m_Desc.ArraySize)
		return E_FAIL;

	const D3D11_TEXTURE2D_DESC& srcDesc = _SrcTexture->GetDesc();

	DirectX::ScratchImage srcImage;
	if (FAILED(DirectX::CaptureTexture(DEVICE, CONTEXT,
		_SrcTexture->GetTex2D().Get(),
		srcImage)))
	{
		return E_FAIL;
	}

	DirectX::ScratchImage processedImage;

	// 포맷 변환
	const DirectX::Image* finalImage;
	DirectX::TexMetadata finalMetadata;

	// processedImage에 데이터가 있으면 그걸 사용
	if (processedImage.GetImageCount() > 0)
	{
		finalImage = processedImage.GetImages();
		finalMetadata = processedImage.GetMetadata();
	}
	else
	{
		finalImage = srcImage.GetImages();
		finalMetadata = srcImage.GetMetadata();
	}

	// 포맷이 다르면 변환
	if (finalMetadata.format != m_Desc.Format)
	{
		DirectX::ScratchImage converted;

		if (FAILED(DirectX::Convert(
			finalImage,
			1,
			finalMetadata,
			m_Desc.Format,
			DirectX::TEX_FILTER_DEFAULT,
			DirectX::TEX_THRESHOLD_DEFAULT,
			converted)))
		{
			return E_FAIL;
		}

		processedImage = std::move(converted);
		finalImage = processedImage.GetImages();
	}

	// 배열로 복사
	UINT dstSubresource = D3D11CalcSubresource(0, _ArrayIndex, m_Desc.MipLevels);
	CONTEXT->UpdateSubresource(
		m_Tex2D.Get(),
		dstSubresource,
		nullptr,
		finalImage->pixels,
		(UINT)finalImage->rowPitch,
		(UINT)finalImage->slicePitch
	);

	return S_OK;
}

Ptr<CTexture> CTexture::CreateResized(UINT _TargetWidth, UINT _TargetHeight)
{
	if (this == nullptr)
	{
		return nullptr;
	}

	const D3D11_TEXTURE2D_DESC& srcDesc = GetDesc();

	// 크기가 이미 같으면 자기자신 반환
	if (srcDesc.Width == _TargetWidth && srcDesc.Height == _TargetHeight)
	{
		return this;
	}

	// DirectXTex로 리사이즈
	DirectX::ScratchImage srcImage;
	if (FAILED(DirectX::CaptureTexture(DEVICE, CONTEXT, m_Tex2D.Get(), srcImage)))
	{
		return nullptr;
	}

	DirectX::ScratchImage resizedImage;
	if (FAILED(DirectX::Resize(srcImage.GetImages(),
		srcImage.GetImageCount(),
		srcImage.GetMetadata(),
		_TargetWidth,
		_TargetHeight,
		DirectX::TEX_FILTER_LINEAR,
		resizedImage)))
	{
		return nullptr;
	}

	// 새 텍스처 생성
	ComPtr<ID3D11Texture2D> pNewTex;
	if (FAILED(DirectX::CreateTexture(DEVICE,
		resizedImage.GetImages(),
		resizedImage.GetImageCount(),
		resizedImage.GetMetadata(),
		(ID3D11Resource**)pNewTex.GetAddressOf())))
	{
		return nullptr;
	}

	// CTexture 래핑
	Ptr<CTexture> pResult = new CTexture;
	pResult->m_Tex2D = pNewTex;

	// SRV 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = resizedImage.GetMetadata().format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	DEVICE->CreateShaderResourceView(pNewTex.Get(), &srvDesc,
		pResult->m_SRV.GetAddressOf());

	return pResult;
}

int CTexture::Load(const wstring& _FilePath)
{
	path filepath = _FilePath;
	path extention = filepath.extension();

	HRESULT hr = S_OK;

	// *.dds
	if (extention == L".dds" || extention == L".DDS")
	{
		hr = LoadFromDDSFile(_FilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);		
	}

	// *.tga
	else if (extention == L".tga" || extention == L".TGA")
	{
		hr = LoadFromTGAFile(_FilePath.c_str(), nullptr, m_Image);
	}

	// Window Image Component (*.bmp, *.png, *.jpg, *.jpeg )
	else if (extention == L".bmp" || extention == L".png" || extention == L".jpg" || extention == L".jpeg"
		 || extention == L".BMP" || extention == L".PNG" || extention == L".JPG" || extention == L".JPEG")
	{
		hr = LoadFromWICFile(_FilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	// 알 수 없는 포맷인 경우
	else
	{
		return E_FAIL;
	}
	
	// 로딩이 실패했다면
	if (FAILED(hr))
	{
		errno_t ErrNum = GetLastError();
		wchar_t szBuff[255] = {};
		wsprintf(szBuff, L"Error Number : %d", ErrNum);
		MessageBox(nullptr, szBuff, L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// SystemMem(ScratchImage) -> GPU(Texture2D)
	CreateShaderResourceView( DEVICE
							, m_Image.GetImages()
							, m_Image.GetImageCount()
							, m_Image.GetMetadata()
							, m_SRV.GetAddressOf());

	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	m_Tex2D->GetDesc(&m_Desc);

	return S_OK;
}

int CTexture::Save(const wstring& _FilePath)
{
	return 0;
}



// RENDER_TARGET | SHADER_RESOURCE
// DEPTH_STENCIL
// SHADER_RESOURCE | UNORDERED_ACESS
int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _Flags, D3D11_USAGE _Usage)
{
	m_Desc.Width = _Width; // DepthStencil 텍스쳐는 렌더타겟 해상도와 반드시 일치해야한다.
	m_Desc.Height = _Height;
	m_Desc.Format = _PixelFormat; // Depth 24bit , Stencil 8bit
	m_Desc.ArraySize = 1;
	m_Desc.BindFlags = _Flags;

	m_Desc.Usage = _Usage;   // System Memory 와의 연계 설정

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.CPUAccessFlags = 0;
	}	

	m_Desc.MiscFlags = 0;
	m_Desc.MipLevels = 1;   // 열화버전 해상도 이미지 추가 생성

	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{		
		return E_FAIL;
	}

	// View 생성
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
	}

	return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{	
	m_Tex2D = _Tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	// View 생성
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
	}

	return S_OK;
}
