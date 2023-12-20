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
	int level;
	float speed = 0.2f;
	float speedy = 0.5f;//el programa pasa de metros a píxeles
	float timer = 0.0f;
	bool jump = false;
	bool isPraying = false;
	bool atacking = false;
	bool dead = false;
	bool isFacingRight = false;
	bool godmode = false;
	bool life = true;
	Uint32 atacktimer = 0;
	Uint32 deathtimer = 0;
	Uint32 currentTime = 0;
	Uint32 atackduration = 0;
	Uint32 deathduration = 0;
	const char* texturePath;
	SDL_Texture* texture = NULL;
	
	PhysBody* pbody;
	PhysBody* pbody2;
	int pickCoinFxId;
	int swordfx;
	int saltofx;
	int muertefx;
	int hello;
	bool isWalking = false;
	bool destroybody = false;
	bool right = false;
	bool left = true;
	int posicionanterior;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation idleleft;
	Animation Runright;
	Animation Runleft;
	Animation Pray;
	Animation Atack1;
	Animation Atack1left;
	Animation Death;
	Animation Jump;


};

#endif // __PLAYER_H__