#include "coins.h"
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

Coin::Coin() : Entity(EntityType::COIN)
{

	name.Create("Coin");
}

Coin::~Coin() {

}

bool Coin::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	


	return true;
}

bool Coin::Start() {

	idle.LoadAnimations("idle", "Coin");
	pick = app->audio->LoadFx(parameters.child("coinfx").attribute("path").as_string());
	
	texture = app->tex->Load(texturePath);


	pbody = app->physics->CreateCircle(position.x , position.y, 16, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::COIN;

	


	currentAnimation = &idle;
	return true;
}

bool Coin::Update(float dt)

{
	if (!dead)
	{
		currentAnimation = &idle;

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		app->render->DrawTexture(texture, position.x , position.y , &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	
		
		
	}
	if (dead)
	{
		pbody->body->SetActive(false);
	}
	return true;
}

bool Coin::CleanUp()
{
	app->tex->UnLoad(texture);
	
	return true;
}



void Coin::OnCollision(PhysBody* physA, PhysBody* physB) {


	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		dead = true;
		app->audio->PlayFx(pick);
		
		LOG("Collision PLATFORM");
		break;
	}

}

void Coin::ResetEntity()
{

	dead = false;
	pbody->body->SetActive(true);
}