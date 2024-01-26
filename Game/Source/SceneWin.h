#ifndef __SCENEWIN_H__
#define __SCENEWIN_H__

#include "Module.h"

struct SDL_Texture;

class SceneWin : public Module
{
public:

	SceneWin(App* app, bool start_enabled = true);

	// Destructor
	virtual ~SceneWin();

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

	SDL_Texture* youwin;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	int timer;


};

#endif // __SCENEWIN_H__


