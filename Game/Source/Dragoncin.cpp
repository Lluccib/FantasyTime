#include "Dragoncin.h"
#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Drake::Drake() : Entity(EntityType::PLAYER)
{

	name.Create("drake");
}

Drake::~Drake() {

}

bool Drake::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Drake::Start() {

	Run.LoadAnimations("Idle", "drake");
	Runleft.LoadAnimations("Idleleft", "drake");
	aleteofx = app->audio->LoadFx(parameters.child("aleteo").attribute("path").as_string());
	muertedragonfx = app->audio->LoadFx(parameters.child("muertedragonfx").attribute("path").as_string());
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x+180, position.y, 20, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
	pbody->body->SetGravityScale(0);




	return true;
}

bool Drake::Update(float dt)
{
	/*currentVelocity.y = 0.5f;*/
	currentAnimation = &Runleft;

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	app->render->DrawTexture(texture, position.x-130, position.y-80, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();




	return true;




}

bool Drake::CleanUp()
{

	return true;
}

void Drake::OnCollision(PhysBody* physA, PhysBody* physB) {


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