#ifndef __GHOST_H__
#define __GHOST_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Ghost : public Entity
{
public:

	Ghost();

	virtual ~Ghost();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Move(const iPoint& origin, const iPoint& destination);

public:
	int level;
	int lifes = 3;
	float speed = 0.2f;
	float speedy = 0.5f;//el programa pasa de metros a p�xeles
	float timer = 0.0f;
	int distance;
	int initialpos;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* pathTexture;
	PhysBody* pbody;
	b2Vec2 velocity;

	iPoint playerPos;//posici�n en tiles del player
	iPoint ghostPos;//posici�n en tiles NightBringer
	iPoint nextTilePath;//Siguiente posici�n en tiles.

	bool isWalking = false;
	bool atacking = false;
	bool explosion = false;
	bool dead = false;
	bool left;
	bool right;
	bool bounce;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation atack;

	int muertefantasma;
};

#endif // __PLAYER_H__