#include "heals.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Map.h"

Heal::Heal() : Entity(EntityType::HEAL)
{

	name.Create("Heal");
}

Heal::~Heal() {

}

bool Heal::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();



	return true;
}

bool Heal::Start() {

	
	pick = app->audio->LoadFx(parameters.child("coinfx").attribute("path").as_string());

	texture = app->tex->Load(texturePath);


	pbody = app->physics->CreateCircle(position.x, position.y, 16, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::HEAL;




	currentAnimation = &idle;
	return true;
}

bool Heal::Update(float dt)

{
	if (!dead)
	{
		

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		app->render->DrawTexture(texture, position.x, position.y, NULL);
		



	}
	if (dead)
	{
		pbody->body->SetActive(false);
	}
	return true;
}

bool Heal::CleanUp()
{
	app->tex->UnLoad(texture);

	return true;
}



void Heal::OnCollision(PhysBody* physA, PhysBody* physB) {


	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		dead = true;
		app->audio->PlayFx(pick);
		LOG("Collision PLATFORM");
		break;
	}

}

void Heal::ResetEntity()
{

	dead = false;
	pbody->body->SetActive(true);
}