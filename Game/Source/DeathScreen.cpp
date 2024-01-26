#include "DeathScreen.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "Player.h"
#include "Scene.h"
#include "HUD.h"
#include "Map.h"


#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

#include "Defs.h"
#include "Log.h"

DeathScreen::DeathScreen(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene");
}

// Destructor
DeathScreen::~DeathScreen()
{}

// Called before render is available
bool DeathScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading DeathScreen");
	bool ret = true;


	return ret;
}

bool DeathScreen::Start()
{

	LOG("Loading DeathScreen assets");
	bool ret = true;

	/*app->physics->Disable();
	app->map->Disable();
	app->entityManager->Disable();*/

	app->scene->Disable();
	app->entityManager->Disable();
	

	pantallamuerte = app->tex->Load("Assets/Textures/Gameover.png");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;



	/*app->audio->PlayMusic("Assets/Audio/Music/tema_principal.ogg", 0.0f);*/
	return true;
}

// Called each loop iteration
bool DeathScreen::PreUpdate()
{
	return true;
}


// Called each loop iteration
bool DeathScreen::Update(float dt)
{
	//dibujamos
	

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->FadeToBlack(this, (Module*)app->sceneTitle, 60.0f);

	}

	return true;
}

// Called each loop iteration
bool DeathScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	app->render->DrawTexture(pantallamuerte, 0, 0, NULL, SDL_FLIP_NONE, 0);

	return ret;
}

// Called before quitting
bool DeathScreen::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(pantallamuerte);

	return true;
}
