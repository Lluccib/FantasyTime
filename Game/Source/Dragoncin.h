#ifndef __DRAGONCIN_H__
#define __DRAGONCIN_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Drake : public Entity
{
public:

	Drake();

	virtual ~Drake();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:
	
	float speed = 0.2f;
	float speedy = 0.5f;//el programa pasa de metros a píxeles
	float timer = 0.0f;
	int life = 5;
	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;

	bool isWalking = false;

	Animation* currentAnimation = nullptr;
	Animation Run;
	Animation Runleft;

	int aleteofx;
	int muertedragonfx;


};

#endif // __PLAYER_H__