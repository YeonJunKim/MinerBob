#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "Telegram.h"
#include "EntityNames.h"



class Entity
{

private:

	int          m_ID;

	static int  m_iNextValidID;

	//used by the constructor to give each entity a unique ID
	void SetID(int val);

public:

	Entity(int id)
	{
		SetID(id);
	}

	virtual ~Entity()
	{}

	//all entities must implement an update function
	virtual void  Update() = 0;

	//accessors
	int         ID()const { return m_ID; }
	virtual bool  HandleMessage(const Telegram& msg) = 0;

	string Name()
	{
		return GetNameOfEntity(m_ID);
	}
};


int Entity::m_iNextValidID = 0;

//----------------------------- SetID -----------------------------------------
//
//  this must be called within each constructor to make sure the ID is set
//  correctly. It verifies that the value passed to the method is greater
//  or equal to the next valid ID, before setting the ID and incrementing
//  the next valid ID
//-----------------------------------------------------------------------------
void Entity::SetID(int val)
{
	m_ID = val;

	m_iNextValidID = m_ID + 1;
}

#endif