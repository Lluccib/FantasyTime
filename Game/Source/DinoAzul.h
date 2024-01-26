#ifndef __DINO_AZUL_H__
#define __DINO_AZUL_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"
struct SDL_Texture;

class Blue : public Entity
{
public:

	Blue();

	virtual ~Blue();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Move(const iPoint& origin, const iPoint& destination);

public:
	int level;
	int life = 3;
	float speed = 0.2f;
	float speedy = 0.5f;//el programa pasa de metros a píxeles
	float timer = 0.0f;
	float distance;
	bool dead = false;
	bool right = true;
	int initialIdlePosition;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	const DynArray<iPoint>* Path;
	SDL_Texture* pathTexture;
	iPoint playerTilePos;//posición en tiles del player
	iPoint BlueTilePos;//posición en tiles NightBringer
	iPoint nextTilePath;//Siguiente posición en tiles.
	b2Vec2 velocity;
	PhysBody* pbody;
	
	
	bool isWalking = false;
	bool atacking = false;
	bool bounce;
	bool agro = false;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation walk;
	Animation damage;
	Animation run;
	

	int raptoratack;
	int raptordeath;
	bool deathfx = true;

};

#endif // __PLAYER_H__