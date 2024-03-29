#include "SceneTitle.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"


#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

#include "Defs.h"
#include "Log.h"

SceneTitle::SceneTitle(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene");
}

// Destructor
SceneTitle::~SceneTitle()
{}

// Called before render is available
bool SceneTitle::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneTitle");
	bool ret = true;


	return ret;
}

bool SceneTitle::Start()
{

	LOG("Loading SceneTitle assets");
	bool ret = true;


	titlescreen = app->tex->Load("Assets/Textures/Titlescreen.png");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;



	/*app->audio->PlayMusic("Assets/Audio/Music/tema_principal.ogg", 0.0f);*/
	return true;
}

// Called each loop iteration
bool SceneTitle::PreUpdate()
{
	return true;
}


// Called each loop iteration
bool SceneTitle::Update(float dt)
{
	//dibujamos
	SDL_Rect Recttitlescreen{ 0, 0, windowW, windowH };
	app->render->DrawTexture(titlescreen, 0, 0, NULL, SDL_FLIP_NONE, 0);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->FadeToBlack(this, (Module*)app->scene, 60.0f);

	}
	return true;
}

// Called each loop iteration
bool SceneTitle::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneTitle::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(titlescreen);

	return true;
}