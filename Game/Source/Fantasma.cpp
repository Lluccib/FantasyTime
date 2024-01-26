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
#include "EntityManager.h"
#include "Map.h"

Ghost::Ghost() : Entity(EntityType::GHOST)
{

	name.Create("ghost2");
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
	atack.LoadAnimations("explosion", "ghost");
	pathTexture = app->tex->Load("Assets/Textures/path.png");
	muertefantasma = app->audio->LoadFx(parameters.child("muertefantasma").attribute("path").as_string());
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 20, position.y, 12, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;
	initialpos = position.x;

	pbody->body->SetGravityScale(0);

	return true;
}

bool Ghost::Update(float dt)
{
	/*currentVelocity.y = 0.5f;*/
	if (!dead)
	{
		if (tp)
		{
			tp = false;
			pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		}
		playerPos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
		ghostPos = app->map->WorldToMap(position.x, position.y);

		distance = playerPos.DistanceTo(ghostPos);
		if (app->scene->player->position.x < position.x)
		{
			right = false;
		}
		else
		{
			right = true;
		}
		if (distance < 7)
		{
			app->map->pathfinding->CreatePath(ghostPos, playerPos);

			if (distance >= 0 )
			{
				atacking = true;
				const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
				if (path->Count() > 1) {
					nextTilePath = { path->At(1)->x, path->At(1)->y };
					Move(ghostPos, playerPos);
				}
				currentAnimation = &idle;
			}
			else 
			{
				currentAnimation = &idle;
				velocity = { 0, 0};
				app->map->pathfinding->ClearLastPath();
			}
		}
		else
		{
			const int idleDistance = 3;

			if (position.x >= initialpos + idleDistance * 32)
			{
				bounce = false;
			}
			else if (position.x <= initialpos - idleDistance * 32)
			{
				bounce = true;
			}

			velocity.x = bounce ? 1 : -1;
			position.x += velocity.x;
			currentAnimation = &idle;
		}
		if (explosion)
		{
			currentAnimation = &atack;
			
		}
		if (currentAnimation == &atack && currentAnimation->HasFinished())
		{
			
			explosion = false;
			currentAnimation = &idle;
			currentAnimation->Reset();
		}

		
	}

	if (dead)
	{
		
		velocity = { 0,0 };
		dead = false;
		pbody->body->SetActive(false);
		app->audio->PlayFx(muertefantasma);
		/*enemyPbody->body->SetActive(false)*/;
	}

	
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	pbody->body->SetLinearVelocity(velocity);
	/*enemyPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);*/

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	if (atacking)
	{
		if (right) app->render->DrawTexture(texture, position.x - 20, position.y - 20, &rect);
		else app->render->DrawTexture(texture, position.x - 20, position.y - 20, &rect, SDL_FLIP_HORIZONTAL);
	}
	else {
		if (bounce) app->render->DrawTexture(texture, position.x - 20, position.y - 20, &rect);
		else app->render->DrawTexture(texture, position.x - 20, position.y - 20, &rect, SDL_FLIP_HORIZONTAL);
	}
	

	currentAnimation->Update();

	if (app->physics->debug)
	{
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(pathTexture, pos.x, pos.y);
		}
	}
	

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	


	return true;




}

bool Ghost::CleanUp()
{

	return true;
}

void Ghost::Move(const iPoint& origin, const iPoint& destination)
{
	float xDiff = destination.x - origin.x;
	float yDiff = destination.y - origin.y;

	if (app->map->pathfinding->IsWalkable(destination) != 0)
	{
		velocity.x = (xDiff < 0) ? -1 : (xDiff > 0) ? 1 : 0;
		velocity.y = (yDiff < 0) ? -2 : (yDiff > 0) ? 2: 0;
	}
	else {
		velocity = { 0, 0 };
	}
}
void Ghost::OnCollision(PhysBody* physA, PhysBody* physB) {


	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:

		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYERATACK:
		dead = true;
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		explosion = true;
		
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}
