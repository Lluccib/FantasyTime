#include "SceneIntro.h"
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

SceneIntro::SceneIntro(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene");
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneIntro");
	bool ret = true;


	return ret;
}

bool SceneIntro::Start()
{
	
	LOG("Loading SceneIntro assets");
	bool ret = true;

	
	logo = app->tex->Load("Assets/Textures/DreamSphere.png");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	timer = 0.0f;

	/*app->audio->PlayMusic("Assets/Audio/Music/tema_principal.ogg", 0.0f);*/
	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}


// Called each loop iteration
bool SceneIntro::Update(float dt)
{
	//dibujamos
	SDL_Rect Recttitlescreen{ 0, 0, windowW, windowH };
	app->render->DrawTexture(logo, 0, 0, NULL, SDL_FLIP_NONE, 0);
	
	timer += dt * 1000; // Convertir dt a milisegundos
	
	if (timer >= 3000) {
		app->fade->FadeToBlack(this, (Module*)app->sceneTitle, 60.0f);
	}

	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(logo);

	return true;
}
