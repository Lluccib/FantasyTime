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

Player::Player() : Entity(EntityType::PLAYER)
{

	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	idle.LoadAnimations("idle","player");
	idleleft.LoadAnimations("idleleft", "player");
	Runright.LoadAnimations("Runright","player");
	Runleft.LoadAnimations("Runleft", "player");
	Pray.LoadAnimations("Pray", "player");
	Atack1.LoadAnimations("Atack1", "player");
	Atack1left.LoadAnimations("Atack1left", "player");
	Death.LoadAnimations("Death", "player");
	Jump.LoadAnimations("Jump", "player");
	
	
	texture = app->tex->Load(texturePath);
	currentAnimation = &idle;

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	level = 1;
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");


	return true;
}

bool Player::Update(float dt)
{
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	

	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	/*currentVelocity.y = 0.5f;*/


	if (life, !isWalking, !jump, !dead, !atacking)
	{
		currentAnimation = &idle;
		
	}
	
	if (!life)
	{
		currentAnimation = &Death;
	}


	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE )
	{
		isWalking = false;
		
		currentVelocity.x = 0;
		

	}


	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jump && !dead && !godmode) {
		jump = true;

		currentAnimation = &Jump;
		currentVelocity.y = -0.35 * dt;
		pbody->body->SetLinearVelocity(currentVelocity);
		
	}

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && !jump && !dead && !godmode && !isWalking) {
		
		

		if (right)
		{
			atacking = true;
			currentAnimation = &Atack1;
			pbody2 = app->physics->CreateRectangleSensor(position.x + 40, position.y + 16, 8, 32, bodyType::STATIC);
			pbody2->ctype = ColliderType::PLAYERATACK;
			pbody2->listener = this;
		}
		else if (left)
		{
			atacking = true;
			currentAnimation = &Atack1;
			pbody2 = app->physics->CreateRectangleSensor(position.x - 10, position.y + 16, 8, 32, bodyType::STATIC);
			pbody2->ctype = ColliderType::PLAYERATACK;
			pbody2->listener = this;
		}
		atacktimer = SDL_GetTicks();
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && godmode == true) {
		currentVelocity.y = speed * dt;
		
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && godmode == true) {
		currentVelocity.y = -speed * dt;
		
	}
	

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !dead) {
		currentVelocity.x = -speed * dt;
		isWalking = true;
		currentAnimation = &Runright;
		atacking = false;
		float camSpeed = 0.2f;
		right = false;
		left = true;

	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead) {
		currentVelocity.x = speed * dt;
		isWalking = true;
		currentAnimation = &Runright;
		atacking = false;
		float camSpeed = 0.2f;
		left = false;
		right = true;
		
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !dead) {

		dead = true;
		life = false;
		currentAnimation = &Death;
		deathtimer = SDL_GetTicks();
	}



	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) //START FROM FIRST LEVEL
	{
		dead = false;
		pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 4), PIXEL_TO_METERS(32 * 26) }, 0);

		level = 1;

		app->render->camera.x = 0;
		app->render->camera.y = -190;

		
		

	}

	//if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) //START FROM SECOND LEVEL
	//{
	//	dead = false;
	//	pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 145), PIXEL_TO_METERS(32 * 16) }, 0);

	//	level = 2;

	//	app->render->camera.x = 0;
	//	app->render->camera.y = -190;


	//}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		dead = false;
		if (level == 1)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 4), PIXEL_TO_METERS(32 * 26) }, 0);
		}
		else
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 145), PIXEL_TO_METERS(32 * 16) }, 0);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) { //ENTER GOD MODE

		godmode = !godmode;

		


	}
	if (destroybody)
	{
		destroybody = false;
		pbody2->body->GetWorld()->DestroyBody(pbody2->body);
	}
	if (currentAnimation == &Atack1 && currentAnimation->HasFinished())
	{

		atacking = false;
		destroybody = true;
		currentAnimation->Reset();
		
		
	}
	else if (currentAnimation == &Atack1left && currentAnimation->HasFinished())
	{

		atacking = false;
		destroybody = true;
		currentAnimation->Reset();


	}
	else

	if (godmode)
	{

		pbody->body->GetFixtureList()->SetSensor(true);
		pbody->body->SetGravityScale(0);

	}
	else
	{
		pbody->body->GetFixtureList()->SetSensor(false);
		pbody->body->SetGravityScale(1);
		
	}


	//Set the velocity of the pbody of the player

	if (jump == false && !godmode) {
		currentVelocity.y = -GRAVITY_Y	;
		
	}
	//if (atacking)
	//{
	//	currentTime = SDL_GetTicks();
	//	atackduration = currentTime - atacktimer;
	//	if (atackduration >= 700) //700
	//	{
	//		atacking = false;
	//		currentAnimation->Reset();
	//	}
	//}
	if (dead)
	{
		currentTime = SDL_GetTicks();
		deathduration = currentTime - deathtimer;
		if (deathduration >= 1500) //700
		{
			
			pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 4), PIXEL_TO_METERS(32 * 26) }, 0);
			dead = false;
			life = true;
			app->render->camera.x = 0;
			app->render->camera.y = -190;

			currentAnimation->Reset();
			

		}
	}
	

	

	pbody->body->SetLinearVelocity(currentVelocity);
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	if (left)
	{
		app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(),SDL_FLIP_HORIZONTAL);
		currentAnimation->Update();
	}
	if (right)
	{
		app->render->DrawTexture(texture, position.x-5, position.y, &currentAnimation->GetCurrentFrame());
		currentAnimation->Update();
	}
	
	
	//Movimiento de la camara, y bloqueo de la camara
	if (app->render->camera.x - position.x -100 <= -200 && app->render->camera.x - position.x -100 >= -12850) {
		app->render->camera.x = -(position.x - 100);

	}

	//if (app->render->camera.x - position.x - 100 <= -12900) {
	//	app->render->camera.x = -6333;
	//}
	
	printf("\r %i", app->render->camera.x);


	

	
	return true;

	


}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	if (!godmode && physA->ctype == ColliderType::PLAYER)
	{
		if (life)
		{
			switch (physB->ctype)
			{
			case ColliderType::ITEM:
				LOG("Collision ITEM");
				/*app->audio->PlayFx(pickCoinFxId);*/
				break;
			case ColliderType::PLATFORM:
				jump = false;
				LOG("Collision PLATFORM");
				break;

			case ColliderType::ENEMY:
				life = false;
				dead = true;
				deathtimer = SDL_GetTicks();
				LOG("Collision PLATFORM");
				break;
			case ColliderType::UNKNOWN:
				LOG("Collision UNKNOWN");
				break;
			case ColliderType::ENEMYATTACK:
				life = false;
				dead = true;
				deathtimer = SDL_GetTicks();
				LOG("Collision PLATFORM");
				break;
			}
		}
		if (dead)
		{
			switch (physB->ctype)
			{
			case ColliderType::ITEM:
				LOG("Collision ITEM");
				/*app->audio->PlayFx(pickCoinFxId);*/
				break;
			case ColliderType::PLATFORM:
				jump = false;
				LOG("Collision PLATFORM");
				break;
			case ColliderType::UNKNOWN:
				LOG("Collision UNKNOWN");
				break;
			}
		}
		
		
	}
	if (godmode)
	{

	}
	
	
	
}