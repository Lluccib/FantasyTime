#include "SceneWin.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ModuleFadeToBlack.h"
#include "Scene.h"


#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

#include "Defs.h"
#include "Log.h"

SceneWin::SceneWin(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene");
}

// Destructor
SceneWin::~SceneWin()
{}

// Called before render is available
bool SceneWin::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneWin");
	bool ret = true;


	return ret;
}

bool SceneWin::Start()
{

	LOG("Loading SceneWin assets");
	bool ret = true;


	youwin = app->tex->Load("Assets/Textures/Youwin.png");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;


	/*app->audio->PlayMusic("Assets/Audio/Music/tema_principal.ogg", 0.0f);*/
	return true;
}

// Called each loop iteration
bool SceneWin::PreUpdate()
{
	return true;
}


// Called each loop iteration
bool SceneWin::Update(float dt)
{
	//dibujamos

	app->render->DrawTexture(youwin, 0, 0, NULL, SDL_FLIP_NONE, 0);

	return true;
}

// Called each loop iteration
bool SceneWin::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneWin::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(youwin);

	return true;
}
