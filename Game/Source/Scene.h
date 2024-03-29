#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "DinoAzul.h"
#include "Dragoncin.h"
#include "NightBringer.h"
#include "Fantasma.h"
#include "Boss_Golem.h"
#include "Red_dragon.h"
#include "coins.h"
#include "heals.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene();

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


	//player
	Player* player;
	//Enemies
	Bringer bringer;
	Ghost ghost;
	Golem golem;
	iPoint GetPlayerPosition();

	virtual bool LoadState(pugi::xml_node node);
	virtual bool SaveState(pugi::xml_node node);
private:
	SDL_Texture* img;
	SDL_Texture* hoguera;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	List<Entity*> bringerList;
	List<Entity*> ghostList;
	

};

#endif // __SCENE_H__