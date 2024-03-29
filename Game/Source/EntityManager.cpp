#include "EntityManager.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::BLUE:
		entity = new Blue();
		break;
	case EntityType::DRAKE:
		entity = new Drake();
		break;
	case EntityType::NIGHTBRINGER:
		entity = new Bringer();
		break;
	case EntityType::GHOST:
		entity = new Ghost();
		break;
	case EntityType::GOLEM:
		entity = new Golem();
		break;
	case EntityType::DRAGON:
		entity = new Dragon();
		break;
	case EntityType::COIN:
		entity = new Coin();
		break;
	case EntityType::HEAL:
		entity = new Heal();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}
void EntityManager::GetBringer(List<Entity*>& bringerList) const
{
	// Clear the provided list to ensure it's empty before populating it.
	bringerList.Clear();

	// Iterate through the list of entities.
	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		// Check if the current entity is of type "SLIME".
		if (entity->data->type == EntityType::NIGHTBRINGER)
		{
			// If it is a slime, add it to the provided list.
			bringerList.Add(entity->data);
		}
	}
}
void EntityManager::GetGhost(List<Entity*>& ghostList) const
{
	// Clear the provided list to ensure it's empty before populating it.
	ghostList.Clear();

	// Iterate through the list of entities.
	ListItem<Entity*>* entity;

	for (entity = entities.start; entity != NULL; entity = entity->next)
	{
		// Check if the current entity is of type "SLIME".
		if (entity->data->type == EntityType::GHOST)
		{
			// If it is a slime, add it to the provided list.
			ghostList.Add(entity->data);
		}
	}
}