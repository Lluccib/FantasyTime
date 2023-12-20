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
	float speedy = 0.5f;//el programa pasa de metros a píxeles
	float timer = 0.0f;
	int distance;
	int initialpos;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* pathTexture;
	PhysBody* pbody;
	b2Vec2 velocity;

	iPoint playerPos;//posición en tiles del player
	iPoint ghostPos;//posición en tiles NightBringer
	iPoint nextTilePath;//Siguiente posición en tiles.

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