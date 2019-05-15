#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#pragma warning (disable:4786)

#include <map>
#include <cassert>
#include <string>
#include "Entity.h"


class Entity;

//provide easy access
#define EntityMgr EntityManager::Instance()



class EntityManager
{
private:

	typedef std::map<int, Entity*> EntityMap;

private:

	//to facilitate quick lookup the entities are stored in a std::map, in which
	//pointers to entities are cross referenced by their identifying number
	EntityMap m_EntityMap;

	EntityManager() {}

	//copy ctor and assignment should be private
	EntityManager(const EntityManager&);
	EntityManager& operator=(const EntityManager&);

public:

	static EntityManager* Instance();

	//this method stores a pointer to the entity in the std::vector
	//m_Entities at the index position indicated by the entity's ID
	//(makes for faster access)
	void            RegisterEntity(Entity* NewEntity);

	//returns a pointer to the entity with the ID given as a parameter
	Entity* GetEntityFromID(int id)const;

	//this method removes the entity from the list
	void            RemoveEntity(Entity* pEntity);
};


//--------------------------- Instance ----------------------------------------
//
//   this class is a singleton
//-----------------------------------------------------------------------------
EntityManager* EntityManager::Instance()
{
	static EntityManager instance;

	return &instance;
}

//------------------------- GetEntityFromID -----------------------------------
//-----------------------------------------------------------------------------
Entity* EntityManager::GetEntityFromID(int id)const
{
	//find the entity
	EntityMap::const_iterator ent = m_EntityMap.find(id);

	return ent->second;
}

//--------------------------- RemoveEntity ------------------------------------
//-----------------------------------------------------------------------------
void EntityManager::RemoveEntity(Entity* pEntity)
{
	m_EntityMap.erase(m_EntityMap.find(pEntity->ID()));
}

//---------------------------- RegisterEntity ---------------------------------
//-----------------------------------------------------------------------------
void EntityManager::RegisterEntity(Entity* NewEntity)
{
	m_EntityMap.insert(std::make_pair(NewEntity->ID(), NewEntity));
}




#endif