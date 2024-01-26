#include "HUD.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "Scene.h"
#include "Player.h"


#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

#include "Defs.h"
#include "Log.h"

HUD::HUD(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene");
}

// Destructor
HUD::~HUD()
{}

// Called before render is available
bool HUD::Awake(pugi::xml_node& config)
{
	LOG("Loading HUD");
	bool ret = true;


	return ret;
}

bool HUD::Start()
{

	LOG("Loading HUD assets");
	bool ret = true;


	cincovidas = app->tex->Load("Assets/Textures/cincovidas.png");
	cuatrovidas = app->tex->Load("Assets/Textures/cuatrovidas.png");
	tresvidas = app->tex->Load("Assets/Textures/tresvidas.png");
	dosvidas = app->tex->Load("Assets/Textures/dosvidas.png");
	unavida = app->tex->Load("Assets/Textures/unavida.png");


	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;



	/*app->audio->PlayMusic("Assets/Audio/Music/tema_principal.ogg", 0.0f);*/
	return true;
}

// Called each loop iteration
bool HUD::PreUpdate()
{
	return true;
}


// Called each loop iteration
bool HUD::Update(float dt)
{
	//dibujamos
	//SDL_Rect Recttitlescreen{ 0, 0, windowW, windowH };
	//app->render->DrawTexture(titlescreen, 0, 0, NULL, SDL_FLIP_NONE, 0);

	/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->FadeToBlack(this, (Module*)app->scene, 60.0f);

	}*/
	
	if (app->scene->player->lives == 5)
	{
		app->render->DrawTexture(cincovidas, 0, 0, NULL, SDL_FLIP_NONE, 0);
	}
	if (app->scene->player->lives == 4)
	{
		app->render->DrawTexture(cuatrovidas, 0, 0, NULL, SDL_FLIP_NONE, 0);
	}
	if (app->scene->player->lives == 3)
	{
		app->render->DrawTexture(tresvidas, 0, 0, NULL, SDL_FLIP_NONE, 0);
	}
	if (app->scene->player->lives == 2)
	{
		app->render->DrawTexture(dosvidas, 0, 0, NULL, SDL_FLIP_NONE, 0);
	}
	if (app->scene->player->lives == 1)
	{
		app->render->DrawTexture(unavida, 0, 0, NULL, SDL_FLIP_NONE, 0);
	}


	return true;
}

// Called each loop iteration
bool HUD::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool HUD::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(cincovidas);
	app->tex->UnLoad(cuatrovidas);
	app->tex->UnLoad(tresvidas);
	app->tex->UnLoad(dosvidas);
	app->tex->UnLoad(unavida);


	return true;
}