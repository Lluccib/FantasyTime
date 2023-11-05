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

	idle.LoadAnimations("idle");
	Runright.LoadAnimations("Runright");
	Runleft.LoadAnimations("Runleft");
	Pray.LoadAnimations("Pray");
	Atack1.LoadAnimations("Atack1");
	Death.LoadAnimations("Death");
	Jump.LoadAnimations("Jump");
	
	
	texture = app->tex->Load(texturePath);
	currentAnimation = &idle;

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
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

	if (!isWalking, !jump, !isPraying, !atacking, !dead)
	{
		currentAnimation = &idle;
	}


	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE )
	{
		isWalking = false;
		
		currentVelocity.x = 0;
		

	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jump && !dead) {
		jump = true;

		currentAnimation = &Jump;
		currentVelocity.y = -0.35 * dt;
		pbody->body->SetLinearVelocity(currentVelocity);
		
	}
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !dead) {
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !dead) {
		currentVelocity.x = -speed * dt;
		isWalking = true;
		currentAnimation = &Runleft;
		
		float camSpeed = 0.2f;

	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead) {
		currentVelocity.x = speed * dt;
		isWalking = true;
		currentAnimation = &Runright;
		
		float camSpeed = 0.2f;
		
	}

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && !dead) {
		
		if (!isPraying)
		{
			isPraying = true;
			currentAnimation = &Pray;
		}
		else
		{
			isPraying = false;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !dead) {

		dead = true;
		currentAnimation = &Death;
		deathtimer = SDL_GetTicks();
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !dead) {
		atacking = true;
		currentAnimation = &Atack1;
		atacktimer = SDL_GetTicks();
	}//No funciona

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
	//	pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 4), PIXEL_TO_METERS(32 * 26) }, 0);

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
			/*pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(1800), PIXEL_TO_METERS(3000)), 0);*/
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) { //ENTER GOD MODE

		//godMode = !godMode;
	}



	//Set the velocity of the pbody of the player

	if (jump == false) {
		currentVelocity.y = -GRAVITY_Y	;
		
	}
	if (atacking)
	{
		currentTime = SDL_GetTicks();
		atackduration = currentTime - atacktimer;
		if (atackduration >= 700) //700
		{
			atacking = false;
			currentAnimation->Reset();
		}
	}
	if (dead)
	{
		currentTime = SDL_GetTicks();
		deathduration = currentTime - deathtimer;
		if (deathduration >= 1500) //700
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(32*4), PIXEL_TO_METERS(32*26) }, 0);
			dead = false;

			app->render->camera.x = 0;
			app->render->camera.y = -190;

			currentAnimation->Reset();

			

		}
	}

	if (isFacingRight) {
		flip = SDL_FLIP_NONE; // No se voltea la textura.
	}
	else {
		flip = SDL_FLIP_HORIZONTAL; // Voltea la textura horizontalmente.
	}

	pbody->body->SetLinearVelocity(currentVelocity);
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();
	
	//Movimiento de la camara, y bloqueo de la camara
	if (app->render->camera.x - position.x -100 <= -200 && app->render->camera.x - position.x -100 >= -7532) {
		app->render->camera.x = -(position.x - 100);

	}
	

	printf("\r %i", app->render->camera.x - position.x - 100);

	return true;

	


}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	if (!godmode)
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
			dead = true;
			currentAnimation = &Death;
			deathtimer = SDL_GetTicks();
			LOG("Collision PLATFORM");
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		}
	}
	if (godmode)
	{

	}
	
}