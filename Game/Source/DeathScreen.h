#ifndef __DEATHSCREEN_H__
#define __DEATHSCREEN_H__

#include "Module.h"

struct SDL_Texture;

class DeathScreen : public Module
{
public:

	DeathScreen(App* app, bool start_enabled = true);

	// Destructor
	virtual ~DeathScreen();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


private:

	SDL_Texture* pantallamuerte;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;



};

#endif // __DEATHSCREEN_H__


