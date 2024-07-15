#pragma once
#include "CEntity.h"

class CComponent;
class CRenderComponent;
class CScript;

#define GET_COMPONENT(Type, TYPE) class C##Type* Type() { return (C##Type*)GetComponent(COMPONENT_TYPE::TYPE); }


class CGameObject :
    public CEntity
{
private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;
    vector<CScript*>        m_vecScript;

    CGameObject*            m_Parent;
    vector<CGameObject*>    m_vecChildren;

    int                     m_LayerIdx; // �Ҽ� ���̾��� �ε��� ��ȣ
    bool                    m_Dead;

public:
    void AddComponent(CComponent* _Comopnent);
    CComponent* GetComponent(COMPONENT_TYPE _Type){return m_arrCom[(UINT)_Type];}
    CRenderComponent* GetRenderComponent() { return m_RenderCom; }

    CGameObject* GetParent() { return m_Parent; }
    int GetLayerIdx() { return m_LayerIdx; }
    void AddChild(CGameObject* _ChildObject);
    const vector<CGameObject*>& GetChildren() { return m_vecChildren; }
    const vector<CScript*> GetScripts() { return m_vecScript; }


    bool IsAncestor(CGameObject* _Object);
    bool IsDead() { return m_Dead; }

    void DisconnectWithLayer();

    // Unregister : ����� ����ϴ�, ����ϴ� ������ ����ϴ�
    // Deregister : ��ϵȰ��� ����ϴ�
    void DeregisterChild();    

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(RigidBody, RIGIDBODY);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(FlipBookComponent, FLIPBOOKCOMPONENT);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(Light2D, LIGHT2D);

public:
    void Begin();
    void Tick();
    virtual void FinalTick();
    void Render();

public:
    virtual CGameObject* Clone() { return new CGameObject(*this); }
    CGameObject();
    ~CGameObject();

    friend class CLevel;
    friend class CLayer;
    friend class CTaskMgr;
};

