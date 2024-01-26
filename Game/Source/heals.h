#ifndef __HEALS_H__
#define __HEALS_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

class Heal : public Entity
{
public:

	Heal();

	virtual ~Heal();

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