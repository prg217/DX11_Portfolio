#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

enum class PlayerState
{
	NONE,

	SWING,
	ROLLING,
	LIFT_START,
	LIFT_END,
};

enum class OguAniState
{
	IDLE,
	IDLE_BACK,
	IDLE_LEFT,
	IDLE_RIGHT,
	IDLE_LEFTDOWN,
	IDLE_LEFTUP,
	IDLE_RIGHTDOWN,
	IDLE_RIGHTUP,

	WALK_DOWN,
	WALK_UP,
	WALK_LEFT,
	WALK_RIGHT,
	WALK_LEFTDOWN,
	WALK_LEFTUP,
	WALK_RIGHTDOWN,
	WALK_RIGHTUP,

	RUN_DOWN,
	RUN_UP,
	RUN_LEFT,
	RUN_RIGHT,
	RUN_LEFTDOWN,
	RUN_LEFTUP,
	RUN_RIGHTDOWN,
	RUN_RIGHTUP,

	IDLE_DANCE,
	DANCE,
	
	ROLL_DOWN,
	ROLL_UP,
	ROLL_LEFT,
	ROLL_RIGHT,
	ROLL_LEFTDOWN,
	ROLL_LEFTUP,
	ROLL_RIGHTDOWN,
	ROLL_RIGHTUP,

	SWING_DOWN,
	SWING_UP,
	SWING_LEFT,
	SWING_RIGHT,
	SWING_LEFTDOWN,
	SWING_LEFTUP,
	SWING_RIGHTDOWN,
	SWING_RIGHTUP,

	LIFT_IDLE1_DOWN,
	LIFT_IDLE1_UP,
	LIFT_IDLE1_LEFT,
	LIFT_IDLE1_RIGHT,
	LIFT_IDLE1_LEFTDOWN,
	LIFT_IDLE1_LEFTUP,
	LIFT_IDLE1_RIGHTDOWN,
	LIFT_IDLE1_RIGHTUP,
	LIFT_IDLE2_DOWN,
	LIFT_IDLE2_UP,
	LIFT_IDLE2_LEFT,
	LIFT_IDLE2_RIGHT,
	LIFT_IDLE2_LEFTDOWN,
	LIFT_IDLE2_LEFTUP,
	LIFT_IDLE2_RIGHTDOWN,
	LIFT_IDLE2_RIGHTUP,

	LIFT_WALK_DOWN,
	LIFT_WALK_UP,
	LIFT_WALK_LEFT,
	LIFT_WALK_RIGHT,
	LIFT_WALK_LEFTDOWN,
	LIFT_WALK_LEFTUP,
	LIFT_WALK_RIGHTDOWN,
	LIFT_WALK_RIGHTUP,

	PUSH_DOWN,
	PUSH_UP,
	PUSH_LEFT,
	PUSH_RIGHT,

	HURT_DOWN,
	HURT_UP,
	HURT_LEFT,
	HURT_RIGHT,
	HURT_LEFTDOWN,
	HURT_LEFTUP,
	HURT_RIGHTDOWN,
	HURT_RIGHTUP,
};

class CHPScript;

class CPlayerScript :
    public CScript
{
private:
    Ptr<CTexture>   m_Texture;
	PlayerState		m_CurPS; // ���� ����

	OguAniState		m_CurAS; // ���� �ִϸ��̼�
	OguAniState		m_PreAS; // ���� �ִϸ��̼�

	int				m_StartFrmIdx; // �ִϸ��̼� ���� �ε���

	CHPScript*		m_HPScript;

	bool			m_Hit; // �¾��� ��
	float			m_SaveHitTime;
	float			m_InvincibilityTime; // ���� �ð�
	OguAniState		m_HitPreAS; // �¾��� �� ���� �ִϸ��̼�

	// =======�̵� ���� ������=======
	float           m_Speed;
	float           m_MinSpeed;
	float           m_MaxSpeed;
    bool            m_IsRun;
    int             m_MoveCount; 
	float           m_SaveFinalDiagonalTime;
    float           m_AllowedTime;
	bool			m_IsRunParticle;
	// ==============================
	// =======�����=======
	float           m_SaveFinalActionTime;
	float			m_IdleDanceTime;
	// ======================
	// =======�� ���� ������=======
	float					m_SaveDanceTime;
	float					m_DanceTime;
	bool					m_IsDance;
	vector<CGameObject*>	m_vDanceEffects;
	// ============================
	// =======������=======
	float			m_RollingSpeedMax;
	float			m_RollingSpeed;
	float			m_RollingDeceleration;
	float			m_SaveRollingTime;
	float			m_RollingDelay;
	// ====================
	// =======��ȣ�ۿ�=======
	bool			m_Interaction;

	bool			m_IsPush;
	// ======================

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
	virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
	virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
    
private:
	void Move();
	void AniState();
	void RunParticle();

	void DanceEffect();
	void DanceEffectDelete();

	void AniFinishCheck();

	void Swing();

	void RollingStart();
	void Rolling();
	void RollingParticle();

	void LiftMove();
	void PushMove();

	void HitEffect();

public:
	OguAniState GetCurAS() { return m_CurAS; }
	float GetSpeed() { return m_Speed; }
	PlayerState GetCurPS() { return m_CurPS; }

	void LiftStart();
	void LiftEnd();
	void LiftEndFinish();

	void SetIsPush(bool _IsPush) { m_IsPush = _IsPush; }

	void Hit();
	void Dead();

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};

