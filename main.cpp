#pragma comment(lib,"lua5.1.lib")
#pragma comment(lib,"libluabind.lib")

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <iostream>
#include <Windows.h>

using namespace std;
using namespace luabind;

#include "Entity.h"
#include "MinersWife.h"
#include "Miner.h"
#include "ScriptedStateMachine.h"
#include "EntityManager.h"
#include "MessageDispatcher.h"
#include "EntityNames.h"


void RunLuaScript(lua_State *L, const char *n)
{
	if (luaL_dofile(L, n))
	{
		cout << "Can't open the file or bad Lua script\n";
	}
}


void RegisterMinersStateMachineWithLua(lua_State* pLua)
{
	module(pLua)
		[
			class_<ScriptedStateMachine<Miner> >("ScriptedStateMachine")

			.def("ChangeState", &ScriptedStateMachine<Miner>::ChangeState)
			.def("RevertToPreviousState", &ScriptedStateMachine<Miner>::RevertToPreviousState)
			.def("CurrentState", &ScriptedStateMachine<Miner>::CurrentState)
			.def("PreviousState", &ScriptedStateMachine<Miner>::PreviousState)
			.def("GlobalStateState", &ScriptedStateMachine<Miner>::GlobalState)
			.def("SetCurrentState", &ScriptedStateMachine<Miner>::SetCurrentState)
			.def("SetPreviousState", &ScriptedStateMachine<Miner>::SetPreviousState)
			.def("SetGlobalState", &ScriptedStateMachine<Miner>::SetGlobalState)
		];
}

void RegisterMinersWifeStateMachineWithLua(lua_State* pLua)
{
	module(pLua)
		[
			class_<ScriptedStateMachine<MinersWife> >("ScriptedStateMachine")

			.def("ChangeState", &ScriptedStateMachine<MinersWife>::ChangeState)
			.def("RevertToPreviousState", &ScriptedStateMachine<MinersWife>::RevertToPreviousState)
			.def("CurrentState", &ScriptedStateMachine<MinersWife>::CurrentState)
			.def("PreviousState", &ScriptedStateMachine<MinersWife>::PreviousState)
			.def("GlobalStateState", &ScriptedStateMachine<MinersWife>::GlobalState)
			.def("SetCurrentState", &ScriptedStateMachine<MinersWife>::SetCurrentState)
			.def("SetPreviousState", &ScriptedStateMachine<MinersWife>::SetPreviousState)
			.def("SetGlobalState", &ScriptedStateMachine<MinersWife>::SetGlobalState)
		];
}


void RegisterEntityWithLua(lua_State* pLua)
{
	module(pLua)
		[
			class_<Entity>("Entity")

			.def("Name", &Entity::Name)
			.def("ID", &Entity::ID)
		];
}


void RegisterMinerWithLua(lua_State* pLua)
{
	module(pLua)
		[
			class_<Miner, bases<Entity> >("Miner")
			.def("GetFSM", &Miner::GetFSM)

			.def("GoldCarried", &Miner::GoldCarried)
			.def("SetGoldCarried", &Miner::SetGoldCarried)
			.def("AddToGoldCarried", &Miner::AddToGoldCarried)
			.def("PocketsFull", &Miner::PocketsFull)

			.def("Fatigued", &Miner::Fatigued)
			.def("DecreaseFatigue", &Miner::DecreaseFatigue)
			.def("IncreaseFatigue", &Miner::IncreaseFatigue)

			.def("Wealth", &Miner::Wealth)
			.def("SetWealth", &Miner::SetWealth)
			.def("AddToWealth", &Miner::AddToWealth)

			.def("Thirsty", &Miner::Thirsty)
			.def("BuyAndDrinkAWhiskey", &Miner::BuyAndDrinkAWhiskey)

			.def_readonly("ComfortLevel", &Miner::ComfortLevel)
			.def_readonly("MaxNuggets", &Miner::MaxNuggets)
			.def_readonly("ThirstLevel", &Miner::ThirstLevel)
			.def_readonly("TirednessThreshold", &Miner::TirednessThreshold)

			.def("HandleMessage", &Miner::HandleMessage)
		];
}


void RegisterMinersWifeWithLua(lua_State* pLua)
{
	module(pLua)
		[
			class_<MinersWife, bases<Entity> >("MinersWife")
			.def("GetFSM", &MinersWife::GetFSM)
			.def("Cooking", &MinersWife::Cooking)
			.def("SetCooking", &MinersWife::SetCooking)
			.def("HandleMessage", &MinersWife::HandleMessage)
		];
}


void RegisterDispatch(lua_State* pLua)
{
	module(pLua)
		[
			class_<MessageDispatcher>("MessageDispatcher")
			.def("DispatchMessage", &MessageDispatcher::DispatchMessage)
			.scope
			[
				def("Instance", &MessageDispatcher::Instance) //returns static singleton instance
			]
		];
}

void RegisterTelegram(lua_State* pLua)
{
	module(pLua)
		[
			class_<Telegram>("Telegram")
			.def_readwrite("Msg", &Telegram::Msg)
		];
}



int main()
{
	//create a lua state
	lua_State* pLua = luaL_newstate();


	//open the lua libaries - new in lua5.1
	luaL_openlibs(pLua);

	//open luabind
	open(pLua);

	//bind the relevant classes to Lua
	RegisterEntityWithLua(pLua);
	RegisterMinerWithLua(pLua);
	RegisterMinersWifeWithLua(pLua);
	RegisterMinersStateMachineWithLua(pLua);
	RegisterMinersWifeStateMachineWithLua(pLua);
	RegisterDispatch(pLua);
	RegisterTelegram(pLua);

	//load and run the script
	RunLuaScript(pLua, "StateMachineScript.lua");

	//create a miner
	Miner* bob = new Miner(ent_Miner_Bob);
	MinersWife* elsa = new MinersWife(ent_Elsa);

	EntityMgr->RegisterEntity(bob);
	EntityMgr->RegisterEntity(elsa);

	//grab the global table from the lua state. This will inlclude
	//all the functions and variables defined in the scripts run so far
	//(StateMachineScript.lua in this example)
	object states = globals(pLua);

	//ensure states is a table
	if (type(states) == LUA_TTABLE)
	{
		//make sure Bob's CurrentState object is set to a valid state.
		elsa->GetFSM()->SetCurrentState(states["DoHomework"]);
		elsa->GetFSM()->SetGlobalState(states["GlobalState"]);
		bob->GetFSM()->SetCurrentState(states["GoHomeAndSleepTilRested"]);

		//run him through a few update cycles
		for (int i = 0; i < 100; ++i)
		{
			bob->Update();
			elsa->Update();

			//dispatch any delayed messages
			Dispatch->DispatchDelayedMessages();

			// give some delay
			Sleep(1000);
		}
	}

	delete bob;
	delete elsa;

	return 0;
}