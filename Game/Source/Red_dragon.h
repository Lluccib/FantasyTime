#ifndef __REDDRAGON_H__
#define __REDDRAGON_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

class Dragon : public Entity
{
public:

	Dragon();

	virtual ~Dragon();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void ResetEntity();

	void Move(const iPoint& origin, const iPoint& destination);

public:
	int level;
	int lives = 10;
	float speed = 0.2f;
	float timer = 0.0f;
	float distance;
	int initialIdlePosition;
	const DynArray<iPoint>* Path;
	b2Vec2 velocity;
	uint currentTime;
	uint atackTimer;
	uint atackduration;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* pathTexture;

	iPoint playerTilePos;//posici�n en tiles del player
	iPoint DragonTilePos;//posici�n en tiles NightBringer
	iPoint nextTilePath;//Siguiente posici�n en tiles.

	PhysBody* pbody;
	//PhysBody* enemyPbody;
	b2Transform initialTransform;
	PhysBody* atackhitbox = NULL;


	bool isWalking = false;
	bool atacking = false;
	bool atackcooldown = false;
	bool dead = false;
	bool left = false;
	bool right = true;
	bool destroybody = false;

	bool attackBodyCreated = false;
	bool destroyAttackBody = false;
	bool hasAtacked = false;
	bool bounce;
	bool agro = false;

	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation atack;
	Animation death;

	int dragondeath;
	int dragonatack;
	bool deathfx = true;


};

#endif // __PLAYER_H__