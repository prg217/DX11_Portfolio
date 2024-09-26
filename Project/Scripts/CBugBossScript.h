#pragma once
#include <Engine/CScript.h>

enum class BugBossAni
{
    Appeared, // ����
    Idle,
    StandAttack,

    Wing, // ���� ���
    WingIdle,
    WingAttackReady,
    WingAttack,

    Down, // �˸��� �� ������ Ÿ���ϸ� ���ƴٴϴ� ������ ��������
    Dead, // ����
};

enum class BugBossPhase
{
    None,
    Appeared,

    Phase1, // ���� �°� ƨ������ ����, �¹޾� ĥ �� �ִ� ���� �����ư��� 5�ʸ��� ���� / 2�� Ÿ�� �� 2��
    Phase2, // Ư���� ������ ���ƴٴ�, �˸��� �� ������ Ÿ���ϸ� ���ƴٴϴ� ������ �������� / 11�� Ÿ�� �� 3�� / ī�޶� ������ �����ϱ� / �������� ����Ʈ ����Ʈ ��ٰ� �����ϱ�
    Phase3, // ��ο���, 2��� ���� / 12�� Ÿ�� �� ����
};

class CBugBossScript :
    public CScript
{
private:
    // ���� �ִϸ��̼�
    CGameObject* m_LightObj;
    CGameObject* m_WhiteObj;
    CGameObject* m_WingObj;

    // ���� ��
    CGameObject* m_Light1;
    CGameObject* m_Light2;

    // �� �� ��
    CGameObject* m_ELight1;
    CGameObject* m_ELight2;

    BugBossPhase m_Phase;

    bool m_PhaseIn; // ������ ���� ����

    float m_AppearedTime;
    float m_Phase1Time;

    bool m_IsAttack;
    int m_AttackCount;
    Vec3 m_AttackColor;

    Ptr<CPrefab> m_Phase1Attack0_Obj;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void FlipPlay(int _FliBookIdx, int _FPS, bool _Repeat);

    void Appeared(); // ���� ����
    void Phase1();
    
    void Phase1Attack0();
    void Phase1Attack1();

    void ChargeEffect(Vec3 _Color);

public:
    void Active(); // ���� Ư�� �ݶ��̴��� �����ϸ� ����

public:
    CLONE(CBugBossScript);
    CBugBossScript();
    CBugBossScript(const CBugBossScript& _Origin);
    ~CBugBossScript();
};

