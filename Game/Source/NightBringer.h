#ifndef __NIGHTBRINGER_H__
#define __NIGHTBRINGER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Bringer : public Entity
{
public:

	Bringer();

	virtual ~Bringer();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	int level;
	int lives = 10;
	float speed = 0.2f;
	float timer = 0.0f;
	uint currentTime;
	uint atackTimer;
	uint atackduration;
	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;
	PhysBody* atackhitbox;

	iPoint playerTile;
	iPoint skeletonTile;
	iPoint nextTile;

	bool isWalking = false;
	bool atacking = false;
	bool atackcooldown = false;
	bool dead = false;
	bool left = false;
	bool right = true;
	bool destroybody = false;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation idleleft;
	Animation walk;
	Animation walkleft;
	Animation damageleft;
	Animation damage;
	Animation atack;
	Animation atackleft;
	Animation death;

	


};

#endif // __PLAYER_H__