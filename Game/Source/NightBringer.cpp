#include "NightBringer.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Bringer::Bringer() : Entity(EntityType::NIGHTBRINGER)
{

	name.Create("bringer");
}

Bringer::~Bringer() {

}

bool Bringer::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Bringer::Start() {

	idle.LoadAnimations("Idle", "bringer");
	idleleft.LoadAnimations("Idleleft", "bringer");
	walk.LoadAnimations("walk", "bringer");
	walkleft.LoadAnimations("walkleft", "bringer");
	damageleft.LoadAnimations("damageleft", "bringer");
	damage.LoadAnimations("damage", "bringer");
	atack.LoadAnimations("atack", "bringer");
	atackleft.LoadAnimations("atackleft", "bringer");
	death.LoadAnimations("death", "bringer");
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 20, position.y, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;




	return true;
}

bool Bringer::Update(float dt)
{
	/*currentVelocity.y = 0.5f;*/
	if (!atacking, !isWalking, !dead)
	{
		
			currentAnimation = &idle;
		
		
		/*currentAnimation = &idle;*/
	}
	else if (dead)
	{
		currentAnimation = &death;
	}



	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	/*if (currentAnimation == &idleleft)
	{
		app->render->DrawTexture(texture, position.x , position.y - 50, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}
	if (currentAnimation == &idle)
	{
		app->render->DrawTexture(texture, position.x - 90, position.y - 50, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}*/
	/*int distancia = app->scene->player->position.x - position.x;
	if ( distancia < 0)
	{
		left = true;
		right = false;
		LOG("CACA");
	}
	else if(distancia>0);
	{
		right = true;
		left = false;
		LOG("CACA2");
	}*/
	left = true;
	right = false;
	if (app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
	{
		dead = true;
		currentAnimation = &death;
		

	}
	if (app->scene->player->position.DistanceTo(position) <= 100 && app->scene->player->position.DistanceTo(position) >= 50 && !dead)
	{
		isWalking = true;
		atacking = false;
		

			currentAnimation = &walk;
		
		
		
		LOG("esta caminando");
	}
	else if (app->scene->player->position.DistanceTo(position) <= 50 && !dead)
	{
		if (!atackcooldown)
		{
			atacking = true;
			isWalking = false;
			
			
			atackTimer = SDL_GetTicks();
			
			if (left)
			{
				atackhitbox = app->physics->CreateRectangleSensor(position.x - 50, position.y + 16, 50, 96, bodyType::STATIC);
				atackhitbox->ctype = ColliderType::ENEMY;
				atackhitbox->listener = this;
			}
			if (right)
			{
				
				atackhitbox = app->physics->CreateRectangleSensor(position.x + 90, position.y + 16, 50, 96, bodyType::STATIC);
				atackhitbox->ctype = ColliderType::ENEMY;
				atackhitbox->listener = this;
			}

			currentAnimation = &atack;
		}
		
		
		
		LOG("esta atacando");
	}
	else
	{
		atacking = false;
		isWalking = false;
	}
	
	if (right)
	{
		app->render->DrawTexture(texture, position.x - 20, position.y - 50, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
		currentAnimation->Update();
	}
	if (left)
	{
		app->render->DrawTexture(texture, position.x -90, position.y - 50, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}
		
	if (destroybody)
	{
		atackhitbox->body->SetActive(false);
		destroybody = false;
		
		
	}
	if (currentAnimation == &atack && currentAnimation->HasFinished() && !destroybody)
	{

		atacking = false;
		destroybody = true;
		atackcooldown = true;
		currentAnimation->Reset();

	}
	if (atackcooldown)
	{
		currentTime = SDL_GetTicks();
		atackduration = currentTime - atackTimer;
		if (atackduration >= 2000) //700
		{

			atackcooldown = false;


		}
	}
	return true;




}

bool Bringer::CleanUp()
{

	return true;
}

void Bringer::OnCollision(PhysBody* physA, PhysBody* physB) {


	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:

		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYERATACK:
		dead = true;
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}
