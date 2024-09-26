#pragma once
#include <Engine/CScript.h>

enum class BugBossAni
{
    Appeared, // 등장
    Idle,
    StandAttack,

    Wing, // 날개 펴기
    WingIdle,
    WingAttackReady,
    WingAttack,

    Down, // 알맞은 색 벌레를 타격하면 날아다니던 보스가 주저앉음
    Dead, // 죽음
};

enum class BugBossPhase
{
    None,
    Appeared,

    Phase1, // 벽에 맞고 튕겨지는 구슬, 맞받아 칠 수 있는 돌을 번갈아가며 5초마다 공격 / 2번 타격 시 2페
    Phase2, // 특정한 색으로 날아다님, 알맞은 색 벌레를 타격하면 날아다니던 보스가 주저앉음 / 11번 타격 시 3페 / 카메라 스케일 조정하기 / 보스에게 포인트 라이트 줬다가 삭제하기
    Phase3, // 어두워짐, 2페와 같음 / 12번 타격 시 죽음
};

class CBugBossScript :
    public CScript
{
private:
    // 보스 애니메이션
    CGameObject* m_LightObj;
    CGameObject* m_WhiteObj;
    CGameObject* m_WingObj;

    // 레벨 빛
    CGameObject* m_Light1;
    CGameObject* m_Light2;

    // 두 눈 빛
    CGameObject* m_ELight1;
    CGameObject* m_ELight2;

    BugBossPhase m_Phase;

    bool m_PhaseIn; // 페이즈 최초 진입

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

    void Appeared(); // 등장 연출
    void Phase1();
    
    void Phase1Attack0();
    void Phase1Attack1();

    void ChargeEffect(Vec3 _Color);

public:
    void Active(); // 맵의 특정 콜라이더에 진입하면 시작

public:
    CLONE(CBugBossScript);
    CBugBossScript();
    CBugBossScript(const CBugBossScript& _Origin);
    ~CBugBossScript();
};

