#ifndef SCRIPTEDScriptedStateMachine_H
#define SCRIPTEDScriptedStateMachine_H
#pragma warning (disable : 4786)
//------------------------------------------------------------------------
//
//  Name:   ScriptedStateMachine.h
//
//  Desc:   A simple scripted state machine class. Inherit from this class and 
//          create some states in Lua to give your agents FSM functionality
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

extern "C"
{
#include <lua.h>
}

#include <luabind/luabind.hpp>
#include <cassert>
#include <Windows.h>
#include "Telegram.h"

template <class entity_type>
class ScriptedStateMachine
{

private:

	//pointer to the agent that owns this instance
	entity_type*      m_pOwner;

	//the current state is a lua table of lua functions. A table may be
	//represented in C++ using a luabind::object
	luabind::object   m_CurrentState;
	luabind::object   m_PreviousState;
	luabind::object   m_GlobalState;

public:

	ScriptedStateMachine(entity_type* owner) :m_pOwner(owner) {}

	//use these methods to initialize the FSM
	void SetCurrentState(const luabind::object& s) { m_CurrentState = s; }
	void SetPreviousState(const luabind::object& s) { m_PreviousState = s; }
	void SetGlobalState(const luabind::object& s) { m_GlobalState = s; }

	//call this to update the FSM
	void  Update()
	{
		ChangeTextColor();

		if (m_GlobalState.is_valid())  //this could also be written as 'if(m_CurrentState)'
		{
			m_GlobalState["Execute"](m_pOwner);
		}

		//make sure the state is valid before calling its Execute 'method'
		if (m_CurrentState.is_valid())  //this could also be written as 'if(m_CurrentState)'
		{
			m_CurrentState["Execute"](m_pOwner);
		}
	}

	// change to a new state
	// 이 함수의 매개변수 new_state에 레퍼런스 '&' 뺐음
	// 레퍼런스 형태로 해버릴 경우 RevertToPreviousState에서 ChangeState 할경우
	// new_state는 PreviousState의 레퍼런스라 PreviousState인거나 마찬가지인데  
	// m_PreviousState = m_CurrentState; 에서 new_state도 바껴버림
	// 그래서 같은 State를 계속 도는 무한루프 형상이 발생
	void  ChangeState(const luabind::object new_state)
	{
		ChangeTextColor();

		Sleep(1000);

		m_PreviousState = m_CurrentState;

		//call the exit method of the existing state
		m_CurrentState["Exit"](m_pOwner);

		//change state to the new state
		m_CurrentState = new_state;

		Sleep(1000);

		//call the entry method of the new state
		m_CurrentState["Enter"](m_pOwner);

		Sleep(1000);
	}

	void  RevertToPreviousState()
	{
		ChangeState(m_PreviousState);
	}

	//retrieve the current state
	const luabind::object&  CurrentState()const { return m_CurrentState; }
	const luabind::object&  PreviousState()const { return m_PreviousState; }
	const luabind::object&  GlobalState()const { return m_GlobalState; }

	bool  HandleMessage(const Telegram& msg)
	{
		ChangeTextColor();

		////first see if the current state is valid and that it can handle
		////the message
		if (m_CurrentState && m_CurrentState["OnMessage"])
		{
			m_CurrentState["OnMessage"](m_pOwner, msg);
			return true;
		}

		//if not, and if a global state has been implemented, send 
		//the message to the global state
		if (m_GlobalState && m_GlobalState["OnMessage"])
		{
			m_GlobalState["OnMessage"](m_pOwner, msg);
			return true;
		}

		return false;
	}

	// just for visual 
	void ChangeTextColor()
	{
		if (m_pOwner->Name() == "Miner Bob")
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		else
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
	}
};

#endif

