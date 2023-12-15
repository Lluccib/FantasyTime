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
	walk.LoadAnimations("walk", "bringer");
	damage.LoadAnimations("damage", "bringer");
	run.LoadAnimations("atack", "bringer");
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
	}


	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	app->render->DrawTexture(texture, position.x-90, position.y-50, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();
	if (app->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
	{
		dead = true;
		currentAnimation = &death;


	}
	if (app->scene->player->position.DistanceTo(position) <= 100 && app->scene->player->position.DistanceTo(position) >= 50)
	{
		isWalking = true;
		currentAnimation = &walk;
	}
	else if (app->scene->player->position.DistanceTo(position) <= 50)
	{
		atacking = true;
		currentAnimation = &run;
	}
	else
	{
		atacking = false;
		isWalking = false;
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
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}
