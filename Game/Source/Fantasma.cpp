#include "Fantasma.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Ghost::Ghost() : Entity(EntityType::GHOST)
{

	name.Create("ghost");
}

Ghost::~Ghost() {

}

bool Ghost::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Ghost::Start() {

	idle.LoadAnimations("Idle", "ghost");
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 20, position.y, 12, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;




	return true;
}

bool Ghost::Update(float dt)
{
	/*currentVelocity.y = 0.5f;*/
	
	currentAnimation = &idle;
	

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	app->render->DrawTexture(texture, position.x-20, position.y-20, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();




	return true;




}

bool Ghost::CleanUp()
{

	return true;
}

void Ghost::OnCollision(PhysBody* physA, PhysBody* physB) {


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
