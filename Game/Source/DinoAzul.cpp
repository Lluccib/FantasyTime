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
#include "EntityManager.h"
#include "Map.h"

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
	raptordeath = app->audio->LoadFx(parameters.child("muertebluefx").attribute("path").as_string());
	raptoratack = app->audio->LoadFx(parameters.child("ataquebluefx").attribute("path").as_string());
	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateCircle(position.x + 20, position.y , 8, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;




	return true;
}

bool Blue::Update(float dt)
{
	if (!dead)
	{
		/*if (tp)
		{
			tp = false;
			pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		}*/
		playerTilePos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y + 64);

		BlueTilePos = app->map->WorldToMap(position.x, position.y);


		distance = playerTilePos.DistanceTo(BlueTilePos);
		if (app->scene->player->position.x < position.x) {
			right = false;
		}
		if (app->scene->player->position.x > position.x) {
			right = true;
		}



		if (distance < 8)//SI ESTA DENTRO DEL RANGO DEL JUGADOR
		{
			app->map->pathfinding->CreatePath(BlueTilePos, playerTilePos);
			Path = app->map->pathfinding->GetLastPath();

			agro = true;

			if (Path->Count() > 1) {
				nextTilePath = { Path->At(1)->x, Path->At(1)->y };
				Move(BlueTilePos, nextTilePath);
			}

			if (distance <= 8 )
			{
				atacking = true;
				
				currentAnimation = &run;
				app->audio->PlayFx(raptoratack);
				currentAnimation->ResetLoopCount();
				currentAnimation->Reset();
				velocity = { 0, -GRAVITY_Y };
			
			}	
			
			


			else if (distance > 3 && !atacking)
			{

				if (Path->Count() > 1) {
					nextTilePath = { Path->At(1)->x, Path->At(1)->y };
					Move(BlueTilePos, nextTilePath);
				}
				currentAnimation = &walk;
			}
			else if (!atacking)
			{
				currentAnimation = &idle;
				velocity = { 0, -GRAVITY_Y };
				app->map->pathfinding->ClearLastPath();
			}
		}
		else//SI ESTA FUERA DEL RANGO DEL JUGADOR
		{
			agro = false;
			atacking = false;

			const int idleDistance = 5;

			if (position.x >= initialIdlePosition + idleDistance * 32)

			{
				bounce = false;
			}
			else if (position.x <= initialIdlePosition - idleDistance * 32)
			{
				bounce = true;
			}

			velocity.x = bounce ? 1 : -1;
			position.x += velocity.x;
			currentAnimation = &walk;

			if (Path == app->map->pathfinding->GetLastPath()) app->map->pathfinding->ClearLastPath();
		}

		
		

		

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		pbody->body->SetLinearVelocity(velocity);
		/*enemyPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);*/
		if (agro) {
			if (right) app->render->DrawTexture(texture, position.x , position.y , &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
			else app->render->DrawTexture(texture, position.x , position.y , &currentAnimation->GetCurrentFrame());
		}
		else {
			if (bounce) app->render->DrawTexture(texture, position.x , position.y , &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
			else app->render->DrawTexture(texture, position.x , position.y , &currentAnimation->GetCurrentFrame());
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


	}
	if (dead)
	{
		LOG("Esta muerto00");
		velocity = { 0,0 };
		if (atacking)
		{
			atacking = false;
		}
		pbody->body->SetActive(false);
		if (Path == app->map->pathfinding->GetLastPath()) app->map->pathfinding->ClearLastPath();

	}

	return true;




}
void Blue::Move(const iPoint& origin, const iPoint& destination)
{
	float xDiff = destination.x - origin.x;
	float yDiff = destination.y - origin.y;

	if (app->map->pathfinding->IsWalkable(destination) != 0)
	{
		velocity.x = (xDiff < 0) ? -2 : (xDiff > 0) ? 2 : 0;
		velocity.y = (yDiff < 0) ? -2 : (yDiff > 0) ? -GRAVITY_Y : 0;
	}
	else {
		velocity = { 0, -GRAVITY_Y };
	}
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
	case ColliderType::PLAYERATACK:
	{
		dead = true;
		app->audio->PlayFx(raptordeath);
		break;
	}
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}



}
