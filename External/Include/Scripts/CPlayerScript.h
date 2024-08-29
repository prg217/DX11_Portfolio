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
    float           m_SaveFinalMoveTime;
	float           m_SaveFinalDiagonalTime;
    float           m_AllowedTime;
	OguAniState		m_CurMS; // 현재 MovementState
	// ==============================
	float			m_IdleDanceTime;

	bool			m_IsRunParticle;

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

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};

