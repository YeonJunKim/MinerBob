#ifndef MINER_H
#define MINER_H

#include <string>
#include <cassert>

#include "Entity.h"
#include "ScriptedStateMachine.h"


class Miner : public Entity
{
private:

	//an instance of the state machine class
	ScriptedStateMachine<Miner>*  m_pStateMachine;

	int m_iGoldCarried;
	int m_iMoneyInBank;
	//the higher the value, the thirstier the miner
	int m_iThirst;
	//the higher the value, the more tired the miner
	int m_iFatigue;


public:
	//the amount of gold a miner must have before he feels he can go home
	const int ComfortLevel = 5;
	//the amount of nuggets a miner can carry
	const int MaxNuggets = 3;
	//above this value a miner is thirsty
	const int ThirstLevel = 5;
	//above this value a miner is sleepy
	const int TirednessThreshold = 5;

	Miner(int id) :Entity(id),
		m_iGoldCarried(0),
		m_iMoneyInBank(0),
		m_iThirst(0),
		m_iFatigue(0)
	{
		m_pStateMachine = new ScriptedStateMachine<Miner>(this);
	}

	~Miner() { delete m_pStateMachine; }


	void Update()
	{
		m_iThirst += 1;
		m_pStateMachine->Update();
	}

	virtual bool  HandleMessage(const Telegram& msg);

	ScriptedStateMachine<Miner>*  GetFSM()const { return m_pStateMachine; }


	int           GoldCarried()const { return m_iGoldCarried; }
	void          SetGoldCarried(const int val) { m_iGoldCarried = val; }
	void		  AddToGoldCarried(const int val);

	bool          PocketsFull()const { return m_iGoldCarried >= MaxNuggets; }

	bool          Fatigued()const;
	void          DecreaseFatigue() { m_iFatigue -= 1; }
	void          IncreaseFatigue() { m_iFatigue += 1; }

	int           Wealth()const { return m_iMoneyInBank; }
	void          SetWealth(const int val) { m_iMoneyInBank = val; }
	void          AddToWealth(const int val);

	bool          Thirsty()const;
	void          BuyAndDrinkAWhiskey() { m_iThirst = 0; m_iMoneyInBank -= 2; }
};


void Miner::AddToGoldCarried(const int val)
{
	m_iGoldCarried += val;

	if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

//-----------------------------------------------------------------------------
void Miner::AddToWealth(const int val)
{
	m_iMoneyInBank += val;

	if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

//-----------------------------------------------------------------------------
bool Miner::Thirsty()const
{
	if (m_iThirst >= ThirstLevel) { return true; }

	return false;
}


//-----------------------------------------------------------------------------
bool Miner::Fatigued()const
{
	if (m_iFatigue > TirednessThreshold)
	{
		return true;
	}

	return false;
}


bool Miner::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

#endif
