#pragma once
#include "CComponent.h"

class CRigidBody :
    public CComponent
{
private:
    Vec3    m_Velocity;             // �ӵ� ( ����, ũ�� )
    Vec3    m_VelocityByGravity;    // �߷¿� ���ؼ� �����ϴ� �ӵ�
    Vec3    m_AddVelocity;          // ���� �ӵ��� ������ �ӵ�

    float   m_Mass;                 // ����
    Vec3    m_Force;                // ��

    float   m_InitWalkSpeed;        // �ּ� ���� �ӷ�
    float   m_MaxWalkSpeed;         // �ִ� ���� �ӷ�
    float   m_MaxGravitySpeed;      // �߷����� �߻��� ���ϼӵ� ����

    float   m_Friction;             // ������    

    float   m_GravityAccel;         // �߷� ���ӵ�
    bool    m_UseGravity;
    bool    m_Ground;               // �� ���� ���ִ��� üũ
    float   m_JumpSpeed;            // ���� �ӷ�

    /*
    // Ground On / Off ȣ���ų �Լ�������
    // CallBack
    CALL_BACK   m_GroundFunc;
    CALL_BACK   m_AirFunc;

    // Delegate
    CObj* m_GroundInst;
    DELEGATE    m_GroundDelegate;

    CObj* m_AirInst;
    DELEGATE    m_AirDelegate;
    */
public:
    void AddForce(Vec3 _vForce) { m_Force += _vForce; }

    void SetMass(float _Mass) { m_Mass = _Mass; }
    void SetInitialWalkSpeed(float _Speed) { m_InitWalkSpeed = _Speed; }
    void SetMaxWalkSpeed(float _Speed) { m_MaxWalkSpeed = _Speed; }
    void SetMaxGravitySpeed(float _Speed) { m_MaxGravitySpeed = _Speed; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }
    //void SetJumpSpeed(float _Speed) { m_JumpSpeed = _Speed; }

    //void Jump();
    void SetGravityVelocity(Vec3 _Velocity) { m_VelocityByGravity = _Velocity; }

    float GetMass() { return m_Mass; }
    float GetInitialWalkSpeed() { return m_InitWalkSpeed; }
    float GetMaxWalkSpeed() { return m_MaxWalkSpeed; }
    float GetFriction() { return m_Friction; }
    Vec3 GetGravityVelocity() { return m_VelocityByGravity; }

    //void SetGroundFunc(void (*_pFunc)(void)) { m_GroundFunc = _pFunc; }
    //void SetAirFunc(void(*_pFunc)(void)) { m_AirFunc = _pFunc; }
    /*
    void SetGroundDelegate(CObj* _Inst, DELEGATE _MemFunc)
    {
        m_GroundInst = _Inst;
        m_GroundDelegate = _MemFunc;
    }

    void SetAirDelegate(CObj* _Inst, DELEGATE _MemFunc)
    {
        m_AirInst = _Inst;
        m_AirDelegate = _MemFunc;
    }
    */

    void UseGravity(bool _Use)
    {
        m_UseGravity = _Use;
        if (!m_UseGravity)
            m_VelocityByGravity = Vec3(0.f, 0.f, 0.f);
    }
    
    void SetGround(bool _Ground)
    {
        if (m_Ground == _Ground)
            return;

        m_Ground = _Ground;
        /*
        if (m_Ground)
        {
            m_VelocityByGravity = Vec3(0.f, 0.f, 0.f);

            if (nullptr != m_GroundFunc)
                m_GroundFunc();

            if (m_GroundInst && m_GroundDelegate)
                (m_GroundInst->*m_GroundDelegate)();
        }
        else
        {
            if (nullptr != m_AirFunc)
                m_AirFunc();

            if (m_AirInst && m_AirDelegate)
                (m_AirInst->*m_AirDelegate)();
        }
        */
    }

    bool IsGround() { return m_Ground; }

public:
    virtual void FinalTick() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CRigidBody);
    CRigidBody();
    CRigidBody(const CRigidBody& _Origin);
    ~CRigidBody();
};

