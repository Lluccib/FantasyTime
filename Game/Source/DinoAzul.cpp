#include "DinoAzul.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Blue::Blue() : Entity(EntityType::BLUE)
{

	name.Create("blue");
}

Blue::~Blue() {

}

bool Blue::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Blue::Start() {

	idle.LoadAnimations("Idle", "blue");
	walk.LoadAnimations("walk", "blue");
	damage.LoadAnimations("damage", "blue");
	run.LoadAnimations("run", "blue");
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 20, position.y , 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;




	return true;
}

bool Blue::Update(float dt)
{
	/*currentVelocity.y = 0.5f;*/
	if (!atacking)
	{
		currentAnimation = &walk;
	}
	

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	app->render->DrawTexture(texture, position.x+3, position.y+1, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();
	
	if (app->scene->player->position.DistanceTo(position) <= 100)
	{
		atacking = true;
		currentAnimation = &run;
	}
	else
	{
		atacking = false;
	}


	return true;




}

bool Blue::CleanUp()
{

	return true;
}

void Blue::OnCollision(PhysBody* physA, PhysBody* physB) {

	
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
