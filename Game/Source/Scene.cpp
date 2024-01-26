#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "NightBringer.h"
#include "Fantasma.h"
#include "Boss_Golem.h"
#include "Red_dragon.h"
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include "ModuleFadeToBlack.h"
#include "SceneIntro.h"
#include "HUD.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}
	
	for (pugi::xml_node BlueNode = config.child("blue"); BlueNode; BlueNode = BlueNode.next_sibling("blue"))
	{
		Blue* blue = (Blue*)app->entityManager->CreateEntity(EntityType::BLUE);
		blue->parameters = BlueNode;
	}
	for (pugi::xml_node DrakeNode = config.child("drake"); DrakeNode; DrakeNode = DrakeNode.next_sibling("drake"))
	{
		Drake* drake = (Drake*)app->entityManager->CreateEntity(EntityType::DRAKE);
		drake->parameters = DrakeNode;
	}
	for (pugi::xml_node BringerNode = config.child("bringer"); BringerNode; BringerNode = BringerNode.next_sibling("bringer"))
	{
		Bringer* bringer = (Bringer*)app->entityManager->CreateEntity(EntityType::NIGHTBRINGER);
		bringer->parameters = BringerNode;
	}
	for (pugi::xml_node GhostNode = config.child("ghost"); GhostNode; GhostNode = GhostNode.next_sibling("ghost"))
	{
		Ghost* ghost = (Ghost*)app->entityManager->CreateEntity(EntityType::GHOST);
		ghost->parameters = GhostNode;
	}
	for (pugi::xml_node GhostNode = config.child("Golem"); GhostNode; GhostNode = GhostNode.next_sibling("Golem"))
	{
		Golem* golem = (Golem*)app->entityManager->CreateEntity(EntityType::GOLEM);
		golem->parameters = GhostNode;
	}
	for (pugi::xml_node GhostNode = config.child("dragon"); GhostNode; GhostNode = GhostNode.next_sibling("dragon"))
	{
		Dragon* dragon = (Dragon*)app->entityManager->CreateEntity(EntityType::DRAGON);
		dragon->parameters = GhostNode;
	}
	if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}

	if (config.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = config.child("map").attribute("name").as_string();
		app->map->path = config.child("map").attribute("path").as_string();
	}

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->sceneintro->Disable();
	app->physics->Enable();
	app->map->Enable();
	app->entityManager->Enable();
	app->hud->Enable();
	
	

	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	//app->audio->PlayMusic("Assets/Audio/Music/tema_principal.ogg", 0.0f);
	hoguera = app->tex->Load("Assets/Textures/checkpoint.png");
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

iPoint Scene::GetPlayerPosition(){
	return player->position;
}
// Called each loop iteration
bool Scene::Update(float dt)
{
	/*if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		app->fade->FadeToBlack(this, (Module*)app->sceneintro, 60.0f);

	}*/

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveRequest();
	}

	app->render->DrawTexture(img, (int)textPosX, (int)textPosY);
	
	return true; 
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	app->render->DrawTexture(hoguera, 109*32, 26*32, NULL, SDL_FLIP_NONE);
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
bool Scene::LoadState(pugi::xml_node node) {

	//Player 

		//Player pos
	player->position.x = node.child("playerposition").attribute("x").as_int();
	player->position.y = node.child("playerposition").attribute("y").as_int();
	player->pbody->body->SetTransform({ PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y) }, 0);

	//Player-modes
	
	player->dead = node.child("modes").attribute("dead").as_bool();
	player->right = node.child("modes").attribute("rightmode").as_bool();
	player->left = node.child("modes").attribute("leftmode").as_bool();
	player->godmode = node.child("modes").attribute("God-mode").as_bool();


	//Slime
	for (int bringercount = 0; bringercount < bringerList.Count(); bringercount++) {

		// Retrieve the current slime entity from the list.
		Entity* bringer = bringerList.At(bringercount)->data;

		// Convert the current count to a string for constructing XML attribute names.
		std::string count = std::to_string(bringercount + 1);

		// Update the position of the slime entity based on XML attributes.
		bringer->position.x = node.child(("enemy" + count).c_str()).attribute("x").as_int();
		bringer->position.y = node.child(("enemy" + count).c_str()).attribute("y").as_int();

		// Set the 'tp' (teleport) flag to true for the slime entity.
		bringer->tp = true;
	}

	//Bomber
	//Same with bomber
	for (int ghostcount = 0; ghostcount < ghostList.Count(); ghostcount++) {

		Entity* ghost = ghostList.At(ghostcount)->data;

		std::string count = std::to_string(ghostcount + 1);
		ghost->position.x = node.child(("flyenemy" + count).c_str()).attribute("x").as_int();
		ghost->position.y = node.child(("flyenemy" + count).c_str()).attribute("y").as_int();
		ghost->tp = true;
	}

	return true;
}

bool Scene::SaveState(pugi::xml_node node) {

	//Append on nodes para todo lo que tiene que ver que las entidades del mapa

	//Player

		//Player pos
	pugi::xml_node posnode = node.append_child("playerposition");
	posnode.append_attribute("x").set_value(player->position.x);
	posnode.append_attribute("y").set_value(player->position.y);

	//Player-modes
	pugi::xml_node modesnode = node.append_child("modes");
	modesnode.append_attribute("dead").set_value(player->dead);
	modesnode.append_attribute("rightmode").set_value(player->right);
	modesnode.append_attribute("leftmode").set_value(player->left);
	modesnode.append_attribute("God-mode").set_value(player->godmode);

	


	//Bringer

	for (int bringercount = 0; bringercount < bringerList.Count(); bringercount++) {

		// Convert the current count to a string for constructing XML attribute names.
		std::string hola = std::to_string(bringercount + 1);

		// Append a child node for the current slime entity to the XML node.
		pugi::xml_node enemyNode = node.append_child(("enemy" + hola).c_str());

		// Retrieve the current slime entity from the list.
		Entity* bringer = bringerList.At(bringercount)->data;

		// Load specific information of the slime from the attributes of the nodes.
		enemyNode.append_attribute("x").set_value(bringer->position.x);
		enemyNode.append_attribute("y").set_value(bringer->position.y);
	}

	//Bomber
	for (int ghostcount = 0; ghostcount < ghostList.Count(); ghostcount++) {

		std::string count = std::to_string(ghostcount + 1);
		pugi::xml_node enemyNode = node.append_child(("venemy" + count).c_str());

		Entity* ghost = ghostList.At(ghostcount)->data;

		enemyNode.append_attribute("x").set_value(ghost->position.x);
		enemyNode.append_attribute("y").set_value(ghost->position.y);
	}

	return true;
}
