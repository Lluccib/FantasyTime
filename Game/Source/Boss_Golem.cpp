#include "Boss_Golem.h"
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

Golem::Golem() : Entity(EntityType::GOLEM)
{

	name.Create("Golem");
}

Golem::~Golem() {

}

bool Golem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();


	return true;
}

bool Golem::Start() {


	idle.LoadAnimations("Idle", "Golem");
	walk.LoadAnimations("walk", "Golem");
	atack.LoadAnimations("atack", "Golem");
	death.LoadAnimations("death", "Golem");
	damage.LoadAnimations("damage", "Golem");
	golemdeath = app->audio->LoadFx(parameters.child("bringerdeathfx").attribute("path").as_string());
	golematack = app->audio->LoadFx(parameters.child("bringeratackfx").attribute("path").as_string());
	pathTexture = app->tex->Load("Assets/Textures/path.png");
	texture = app->tex->Load(texturePath);


	pbody = app->physics->CreateCircle(position.x + 20, position.y, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	initialTransform = pbody->body->GetTransform();
	initialIdlePosition = position.x;



	return true;
}

bool Golem::Update(float dt)
{
	//PATHFINDING//
	if (!dead)
	{
		/*if (tp)
		{
			tp = false;
			pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);
		}*/
		playerTilePos = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y + 64);

		GolemTilePos = app->map->WorldToMap(position.x, position.y);


		distance = playerTilePos.DistanceTo(GolemTilePos);
		if (app->scene->player->position.x < position.x) {
			right = false;
		}
		if (app->scene->player->position.x > position.x) {
			right = true;
		}


		if (distance > 9)
		{
			currentAnimation = &idle;
		}
		else if (distance < 8)//SI ESTA DENTRO DEL RANGO DEL JUGADOR
		{
			app->map->pathfinding->CreatePath(GolemTilePos, playerTilePos);
			Path = app->map->pathfinding->GetLastPath();

			agro = true;

			if (Path->Count() > 1) {
				nextTilePath = { Path->At(1)->x, Path->At(1)->y };
				Move(GolemTilePos, nextTilePath);
			}


			if (!atackcooldown) {
				if (!right)
				{
					if (distance <= 3 && !atacking)
					{
						atacking = true;
						hasAtacked = true;
						currentAnimation = &atack;
						app->audio->PlayFx(golematack);
						currentAnimation->ResetLoopCount();
						currentAnimation->Reset();
						velocity = { 0, -GRAVITY_Y };
						atackcooldown = true;
						atackTimer = SDL_GetTicks();
					}
				}
				else {
					if (distance <= 3 && !atacking)
					{
						atacking = true;
						hasAtacked = true;
						currentAnimation = &atack;

						currentAnimation->ResetLoopCount();
						currentAnimation->Reset();
						velocity = { 0, -GRAVITY_Y };
						atackcooldown = true;
						atackTimer = SDL_GetTicks();
					}
				}
			}


			else if (distance > 3 && !atacking)
			{

				if (Path->Count() > 1) {
					nextTilePath = { Path->At(1)->x, Path->At(1)->y };
					Move(GolemTilePos, nextTilePath);
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

			const int idleDistance = 10;

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

		if (atacking)
		{
			if (currentAnimation == &atack && currentAnimation->GetCurrentFrameCount() >= 6 && !attackBodyCreated) {
				if (right) atackhitbox = app->physics->CreateRectangleSensor(position.x + 90, position.y + 16, 50, 40, bodyType::STATIC);
				else atackhitbox = app->physics->CreateRectangleSensor(position.x - 50, position.y + 16, 50, 40, bodyType::STATIC);
				atackhitbox->ctype = ColliderType::ENEMYATTACK;
				attackBodyCreated = true;

			}

			if (currentAnimation == &atack && currentAnimation->GetCurrentFrameCount() >= 9 && attackBodyCreated)
			{
				atacking = false;
				hasAtacked = false;
				attackBodyCreated = false;

				destroyAttackBody = true;

			}
		}
		if (atackcooldown)
		{
			currentTime = SDL_GetTicks();
			atackduration = currentTime - atackTimer;
			if (atackduration >= 3000) //700
			{

				atackcooldown = false;

			}
		}

		if (destroyAttackBody)
		{
			if (atackhitbox != NULL) atackhitbox->body->GetWorld()->DestroyBody(atackhitbox->body);
			atackhitbox = NULL;
			destroyAttackBody = false;
		}

		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		pbody->body->SetLinearVelocity(velocity);
		/*enemyPbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y - PIXEL_TO_METERS(10) }, 0);*/
		if (agro) {
			if (right) app->render->DrawTexture(texture, position.x - 80, position.y - 70, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
			else app->render->DrawTexture(texture, position.x - 90, position.y - 70, &currentAnimation->GetCurrentFrame());
		}
		else {
			if (bounce) app->render->DrawTexture(texture, position.x - 90, position.y - 70, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
			else app->render->DrawTexture(texture, position.x - 90, position.y - 70, &currentAnimation->GetCurrentFrame());
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
		currentAnimation = &death;
		velocity = { 0,0 };
		if (atacking)
		{
			atacking = false;
			destroyAttackBody = true;
		}
		pbody->body->SetActive(false);
		if (Path == app->map->pathfinding->GetLastPath()) app->map->pathfinding->ClearLastPath();

	}

	if (destroyAttackBody)
	{
		if (atackhitbox != NULL) atackhitbox->body->GetWorld()->DestroyBody(atackhitbox->body);
		atackhitbox = NULL;
		destroyAttackBody = false;
	}

	
	return true;
}

bool Golem::CleanUp()
{

	return true;
}

void Golem::Move(const iPoint& origin, const iPoint& destination)
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

void Golem::OnCollision(PhysBody* physA, PhysBody* physB) {


	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:

		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYERATACK:
		lives = lives - app->scene->player->damage;

		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}

}

void Golem::ResetEntity()
{

	dead = false;
	pbody->body->SetActive(true);
}
