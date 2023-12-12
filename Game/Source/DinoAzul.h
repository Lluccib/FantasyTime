#ifndef __DINO_AZUL_H__
#define __DINO_AZUL_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Blue : public Entity
{
public:

	Blue();

	virtual ~Blue();

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
	
	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;
	
	bool isWalking = false;

	Animation* currentAnimation = nullptr;
	Animation Run;


};

#endif // __PLAYER_H__