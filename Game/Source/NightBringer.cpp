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
#include "EntityManager.h"
#include "Map.h"

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
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();


	return true;
}

bool Bringer::Start() {

	
	pathTexture = app->tex->Load("Assets/Textures/path.png");
	texture = app->tex->Load(texturePath);


	pbody = app->physics->CreateCircle(position.x + 20, position.y, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	initialTransform = pbody->body->GetTransform();
	initialIdlePosition = position.x;

	LoadAnimations();

	return true;
}

bool Bringer::Update(float dt)
{

	//PATHFINDING//
	if (!dead)
	{
		playerTilePos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
		NightBringerTilePos = app->map->WorldToMap(position.x, position.y);

		distance = playerTilePos.DistanceTo(NightBringerTilePos);

		if (destroyAttackBody)
		{
			atackhitbox->body->GetWorld()->DestroyBody(atackhitbox->body);
			atackhitbox = NULL;
			destroyAttackBody = false;
		}

		if (distance < 10)
		{
			app->map->pathfinding->CreatePath(NightBringerTilePos, playerTilePos);

			if (distance < 3 && !atacking)
			{
				atacking = true;
				currentAnimation = &atack;

				currentAnimation->ResetLoopCount();
				currentAnimation->Reset();
				velocity = { 0, -GRAVITY_Y };
			}
			else if (distance >= 3 && !atacking)
			{
				const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
				if (path->Count() > 1) {
					nextTilePath = { path->At(1)->x, path->At(1)->y };
					Move(NightBringerTilePos, nextTilePath);
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
		else
		{
			const int idleDistance = 3;

			if (position.x >= initialIdlePosition + idleDistance * 32)
			{
				right = false;
			}
			else if (position.x <= initialIdlePosition - idleDistance * 32)
			{
				right = true;
			}

			velocity.x = right ? 1 : -1;
			position.x += velocity.x;
			currentAnimation = &walk;
		}

		if (atacking)
		{
			if (currentAnimation == &atack && currentAnimation->GetCurrentFrameCount() >= 4 && !attackBodyCreated) {
				if (right) atackhitbox = app->physics->CreateRectangleSensor(position.x + 90, position.y + 16, 50, 96, bodyType::STATIC);
				else atackhitbox = app->physics->CreateRectangleSensor(position.x - 90, position.y + 16, 50, 96, bodyType::STATIC);
				atackhitbox->ctype = ColliderType::ENEMY;
				attackBodyCreated = true;
			}

			if (currentAnimation == &atack && currentAnimation->GetCurrentFrameCount() >= 9 && attackBodyCreated)
			{
				atacking = false;
				attackBodyCreated = false;

				if (atackhitbox != NULL) destroyAttackBody = true;

			}
		}
	}

	if (dead)
	{
		currentAnimation = &death;
		velocity = { 0,0 };
		dead = true;
		pbody->body->SetActive(false);
		/*enemyPbody->body->SetActive(false)*/;
	}


	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	pbody->body->SetLinearVelocity(velocity);
	/*enemyPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);*/

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	if (right) app->render->DrawTexture(texture, position.x - 20, position.y - 50, &rect, SDL_FLIP_HORIZONTAL);
	else app->render->DrawTexture(texture, position.x - 90, position.y - 50, &rect);

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

	return true;
}
	

bool Bringer::CleanUp()
{

	return true;
}

void Bringer::Move(const iPoint& origin, const iPoint& destination) 
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

void Bringer::LoadAnimations()
{
	idle.LoadAnimations("Idle", "bringer");
	idleleft.LoadAnimations("Idleleft", "bringer");
	walk.LoadAnimations("walk", "bringer");
	walkleft.LoadAnimations("walkleft", "bringer");
	damageleft.LoadAnimations("damageleft", "bringer");
	damage.LoadAnimations("damage", "bringer");
	atack.LoadAnimations("atack", "bringer");
	atackleft.LoadAnimations("atackleft", "bringer");
	death.LoadAnimations("death", "bringer");

	currentAnimation = &idle;
}
