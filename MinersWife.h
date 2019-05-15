#ifndef MINERSWIFE_H
#define MINERSWIFE_H

#include <string>
#include <cassert>

#include "Entity.h"
#include "ScriptedStateMachine.h"

class MinersWife : public Entity
{
private:

	//an instance of the state machine class
	ScriptedStateMachine<MinersWife>*  m_pStateMachine;

	//is she presently cooking?
	bool            m_bCooking;

public:

	MinersWife(int id) :Entity(id), m_bCooking(false)
	{
		m_pStateMachine = new ScriptedStateMachine<MinersWife>(this);
	}

	~MinersWife() { delete m_pStateMachine; }


	void Update()
	{
		m_pStateMachine->Update();
	}

	ScriptedStateMachine<MinersWife>*  GetFSM()const { return m_pStateMachine; }


	bool          Cooking()const { return m_bCooking; }
	void          SetCooking(bool val) { m_bCooking = val; }
	bool HandleMessage(const Telegram& msg);
};

bool MinersWife::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}
#endif
