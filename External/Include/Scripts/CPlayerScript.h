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
	ROLL,

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

	// =======�̵� ���� ������=======
	float           m_Speed;
	float           m_MinSpeed;
	float           m_MaxSpeed;
    bool            m_IsRun;
    int             m_MoveCount; 
    float           m_SaveFinalMoveTime;
	float           m_SaveFinalDiagonalTime;
    float           m_AllowedTime;
	OguAniState		m_CurMS; // ���� MovementState
	bool			m_IsRunParticle;
	// ==============================
	// =======�����=======
	float			m_IdleDanceTime;
	// ======================
	// =======ä �ֵθ���=======
	OguAniState		m_PreMS; // ���� MovementState
	bool			m_IsSwing;
	// =========================
	// =======�� ���� ������=======
	float					m_SaveDanceTime;
	float					m_DanceTime;
	vector<CGameObject*>	m_vDanceEffects;
	// ============================

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

	void Swing();
	void SwingFinishCheck();

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};

