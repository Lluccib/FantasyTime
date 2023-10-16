#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	float speed = 0.2f;
	float speedy = 0.5f;//el programa pasa de metros a píxeles
	float timer = 0.0f;
	bool jump = false;
	bool pray = true;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	
	PhysBody* pbody;
	int pickCoinFxId;
	bool isWalking = false;
	
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation Runright;
	Animation Runleft;
	Animation Pray;

};

#endif // __PLAYER_H__