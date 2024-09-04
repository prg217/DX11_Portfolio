#pragma once
#include <Engine/CScript.h>
#include <Engine/define.h>

enum class OguAniState;
class CPlayerScript;

class CSwingObjScript :
    public CScript
{
private:
    CGameObject*    m_pPlayer;
    CPlayerScript*  m_pPlayerScript;

    OguAniState     m_PlayerCurAS;
    bool            m_IsSwing;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    bool GetIsSwing() { return m_IsSwing; }

public:
    CLONE(CSwingObjScript);
    CSwingObjScript();
    //CSwingObjScript(const CSwingObjScript& _Origin);
    ~CSwingObjScript();
};

