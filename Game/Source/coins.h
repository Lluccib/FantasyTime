#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

class Coin : public Entity
{
public:

	Coin();

	virtual ~Coin();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void ResetEntity();

	

public:

	bool dead;
	Animation* currentAnimation = nullptr;
	Animation idle;
	int pick;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	PhysBody* pbody;
};

#endif // __PLAYER_H__