#pragma once
#include "CEntity.h"

class CConstBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>    m_CB;
    D3D11_BUFFER_DESC       m_Desc;
    CB_TYPE                 m_CBType;

public:
    int Create(CB_TYPE _type, UINT _BufferSize);

    void SetData(void* _pData);
    void Binding();
    void Binding_CS();


    virtual CConstBuffer* Clone() { return nullptr; }
public:
    CConstBuffer();
    ~CConstBuffer();
};

