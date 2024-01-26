#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"

struct SDL_Texture;

class HUD : public Module
{
public:

	HUD(App* app, bool start_enabled = true);

	// Destructor
	virtual ~HUD();

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

	SDL_Texture* cincovidas;
	SDL_Texture* cuatrovidas;
	SDL_Texture* tresvidas;
	SDL_Texture* dosvidas;
	SDL_Texture* unavida;

	SDL_Texture* cincocuras;
	SDL_Texture* cuatrocuras;
	SDL_Texture* trescuras;
	SDL_Texture* doscuras;
	SDL_Texture* unacura;
	
	SDL_Texture* cincomonedas;
	SDL_Texture* cuatromonedas;
	SDL_Texture* tresmonedas;
	SDL_Texture* dosmonedas;
	SDL_Texture* unamoneda;

	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;



};

#endif // __HUD_H__




