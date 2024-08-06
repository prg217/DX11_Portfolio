#pragma once
#include "CComponent.h"



class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>          m_Mesh;

    Ptr<CMaterial>      m_Mtrl;         // ���� ������� ����
    Ptr<CMaterial>      m_SharedMtrl;   // ���� ����(������)
    Ptr<CMaterial>      m_DynamicMtrl;  // �ӽ� ����


public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);
    
    Ptr<CMesh> GetMesh() { return m_Mesh; }
    Ptr<CMaterial> GetMaterial() { return m_Mtrl; }
    Ptr<CMaterial> GetSharedMtrl();

    // �������� ���� �� ��ȯ
    Ptr<CMaterial> GetDynamicMaterial();

public:
    virtual void FinalTick() = 0;
    virtual void Render() = 0;
    virtual CRenderComponent* Clone() = 0;


protected:
    void SaveDataToFile(FILE* _File);
    void LoadDataFromFile(FILE* _File);

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    ~CRenderComponent();
};

