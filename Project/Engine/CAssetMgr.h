#pragma once

#include "CPathMgr.h"
#include "CTaskMgr.h"

class CAsset;

class CAssetMgr
	: public CSingleton<CAssetMgr>
{
	SINGLE(CAssetMgr);
private:
	map<wstring, Ptr<CAsset>>	m_mapAsset[(UINT)ASSET_TYPE::END];
	bool						m_Changed;	

public:
	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);

	Ptr<CAsset> FindAsset(ASSET_TYPE _Type, const wstring& _Key);
	//void AddAsset(const wstring& _Key, CAsset* _Asset);

	template<typename T>
	Ptr<T> FindAsset(const wstring& _Key);

	template<typename T>
	void AddAsset(const wstring& _Key, Ptr<T> _Asset);

	// _Flags : D3D11_BIND_FLAG
	Ptr<CTexture> CreateTexture(wstring _strKey, UINT _Width, UINT _Height
								, DXGI_FORMAT _Format, UINT _Flags
								, D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);

	Ptr<CTexture> CreateTexture(wstring _strKey, ComPtr<ID3D11Texture2D> _Tex2D);

public:
	void GetAssetNames(ASSET_TYPE _Type, vector<string>& _vecOut);
	const map<wstring, Ptr<CAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }
	bool IsChanged() { return m_Changed; }

public:
	void Init();
	void Tick();

private:
	void CreateEngineMesh();
	void CreateEngineMaterial();
	void CreateEngineTexture();
	void CreateEngineSprite();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();

private:
	void DeleteAsset(ASSET_TYPE _Type, const wstring& _Key);

	friend class CTaskMgr;
};


template<typename T>
Ptr<T> CAssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	// ���� Ű�� ������ �ִ��� Ȯ��
	Ptr<T> Asset = FindAsset<T>(_Key);

	if (nullptr != Asset)
	{
		return Asset;
	}

	// ���� Ű���� ������ ��������
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	Asset = new T;

	// �ε� ���� �� ����ó��
	if (FAILED(Asset->Load(strFilePath)))
	{		
		MessageBox(nullptr, L"���� �ε� ����", L"�ε� ����", MB_OK);
		return nullptr;
	}

	// Asset �� �ڽ��� Ű���� ��θ� �˰� ��
	Asset->m_Key = _Key;
	Asset->m_RelativePath = _RelativePath;

	// �ʿ� ���
	ASSET_TYPE type = GetAssetType<T>();
	m_mapAsset[(UINT)type].insert(make_pair(_Key, Asset.Get()));

	// Asset ���� �˸�	
	CTaskMgr::GetInst()->AddTask(tTask{ ASSET_CHANGED });

	// �ε��� ���� �ּ� ��ȯ
	return Asset;
}

template<typename T>
Ptr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();

	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);

	if (iter == m_mapAsset[(UINT)Type].end())
		return nullptr;

	//Ptr<CAsset> pAsset;
	//Ptr<CMesh> pMesh = (CMesh*)pAsset.Get()

	return (T*)iter->second.Get();
}

template<typename T>
void CAssetMgr::AddAsset(const wstring& _Key, Ptr<T> _Asset)
{
	ASSET_TYPE Type = GetAssetType<T>();	

	assert(!FindAsset(Type, _Key).Get());

	_Asset->SetKey(_Key);
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, _Asset.Get()));

	// Asset ���� �˸�	
	CTaskMgr::GetInst()->AddTask(tTask{ ASSET_CHANGED });
}

// File �� Asset �������� ���� �ҷ�����
template<typename T>
void SaveAssetRef(Ptr<T> Asset, FILE* _File)
{
	bool bAsset = Asset.Get();
	fwrite(&bAsset, 1, 1, _File);

	if (bAsset)
	{
		SaveWString(Asset->GetKey(), _File);
		SaveWString(Asset->GetRelativePath(), _File);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& Asset, FILE* _File)
{
	bool bAsset = false;
	fread(&bAsset, 1, 1, _File);

	if (bAsset)
	{
		wstring key, relativepath;
		LoadWString(key, _File);
		LoadWString(relativepath, _File);

		Asset = CAssetMgr::GetInst()->Load<T>(key, relativepath);
	}
}