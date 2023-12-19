#ifndef __GHOST_H__
#define __GHOST_H__

#include "Entity.h"
#include "Point.h"
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

public:
	int level;
	int lifes = 3;
	float speed = 0.2f;
	float speedy = 0.5f;//el programa pasa de metros a píxeles
	float timer = 0.0f;

	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;


	bool isWalking = false;
	bool atacking = false;

	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation idleleft;


};

#endif // __PLAYER_H__