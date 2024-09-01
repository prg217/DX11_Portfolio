#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

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
};

class CPlayerScript :
    public CScript
{
private:
    Ptr<CTexture>   m_Texture;

	// =======이동 관련 변수들=======
	float           m_Speed;
	float           m_MinSpeed;
	float           m_MaxSpeed;
    bool            m_IsRun;
    int             m_MoveCount; 
	float           m_SaveFinalDiagonalTime;
    float           m_AllowedTime;
	OguAniState		m_CurAS; // 현재 상태
	bool			m_IsRunParticle;
	// ==============================
	// =======대기모션=======
	float           m_SaveFinalActionTime;
	float			m_IdleDanceTime;
	// ======================
	// =======채 휘두르기=======
	OguAniState		m_PreAS; // 이전 상태
	bool			m_IsSwing;
	// =========================
	// =======춤 관련 변수들=======
	float					m_SaveDanceTime;
	float					m_DanceTime;
	bool					m_IsDance;
	vector<CGameObject*>	m_vDanceEffects;
	// ============================
	// =======구르기=======
	bool			m_IsRolling;
	float			m_RollingSpeedMax;
	float			m_RollingSpeed;
	float			m_RollingDeceleration;
	float			m_SaveRollingTime;
	float			m_RollingDelay;
	// ====================

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

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

public:
	OguAniState GetCurAS() { return m_CurAS; }

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};

