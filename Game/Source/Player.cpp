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
	
	//initilize textures
	//idle
	/*idle.PushBack({ 47, 16, 30, 49 });
	idle.PushBack({ 175, 16, 30, 49 });
	idle.PushBack({ 47, 80, 30, 49 });
	idle.PushBack({ 175, 80, 30, 49 });
	idle.PushBack({ 47, 144, 30, 49 });
	idle.PushBack({ 175, 144, 30, 49 });
	idle.PushBack({ 47, 208, 30, 49 });
	idle.PushBack({ 175, 208, 30, 49 });
	idle.speed = 0.1f;
	idle.loop = true;*/

	//right run
	//Runright.PushBack({ 345, 17, 32, 49 });
	//Runright.PushBack({ 473, 17, 30, 49 });
	//Runright.PushBack({ 345, 80, 30, 49 });
	//Runright.PushBack({ 473, 80, 30, 49 });
	//Runright.PushBack({ 345, 145, 30, 49 });
	//Runright.PushBack({ 471, 145, 30, 49 });
	//Runright.PushBack({ 345, 209, 30, 49 });
	//Runright.PushBack({ 473, 209, 30, 49 });
	//Runright.speed = 0.2f;
	//Runright.loop = true;
	////left run
	//Runleft.PushBack({ 1430, 24, 32, 49 });
	//Runleft.PushBack({ 1558, 24, 30, 49 });
	//Runleft.PushBack({ 1430, 88, 30, 49 });
	//Runleft.PushBack({ 1558, 88, 30, 49 });
	//Runleft.PushBack({ 1430, 153, 30, 49 });
	//Runleft.PushBack({ 1558, 153, 30, 49 });
	//Runleft.PushBack({ 1430, 217, 30, 49 });
	//Runleft.PushBack({ 1558, 217, 30, 49 });
	//Runleft.speed = 0.2f;
	//Runleft.loop = true;
	////pray
	//Pray.PushBack({ 939, 19, 32, 49 });
	//Pray.PushBack({ 1065, 19, 30, 49 });
	//Pray.PushBack({ 1194, 19, 30, 49 });
	//Pray.PushBack({ 1323, 19, 30, 49 });
	//
	//Pray.speed = 0.05f;
	//Pray.loop = false;

	////atack 1
	//Atack1.PushBack({ 43,317,59,54 });
	//Atack1.PushBack({ 169,317,59,54 });
	//Atack1.PushBack({ 295,317,59,54 });
	//Atack1.PushBack({ 428,317,59,54 });
	//Atack1.PushBack({ 553,317,59,54 });
	//Atack1.PushBack({ 676,317,59,54 });
	//Atack1.PushBack({ 808,317,59,54 });
	//Atack1.PushBack({ 937,317,59,54 });
	//Atack1.PushBack({ 43,383,59,54 });
	//Atack1.PushBack({ 167,383,59,54 });
	//Atack1.speed = 0.3f;
	//Atack1.loop = false;

	////Death
	//Death.PushBack({ 1415,343, 60, 50 });
	//Death.PushBack({ 1535,343, 60, 50 });
	//Death.PushBack({ 1415,409, 60, 50 });
	//Death.PushBack({ 1535,409, 60, 50 });
	//Death.speed = 0.2f;
	//Death.loop = false;
	////jump
	//Jump.PushBack({ 642, 14, 44, 52 });
	//Jump.PushBack({ 772, 14, 44, 52 });
	//Jump.PushBack({ 642, 75, 44, 52 });
	//Jump.PushBack({ 772, 75, 44, 52 });
	//Jump.PushBack({ 642, 142, 44, 52 });
	//Jump.PushBack({ 772, 142, 44, 52 });
	//Jump.PushBack({ 642, 206, 44, 52 });
	//Jump.PushBack({ 772, 206, 44, 52 });
	//Jump.speed = 0.3f;
	//Jump.loop = false;
	//load texture
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

	if (!isWalking, !jump, !isPraying, !atacking, !dead)
	{
		currentAnimation = &idle;
	}

	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE )
	{
		isWalking = false;
		
		currentVelocity.x = 0;
		

	}//funciona

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && !jump) {
		jump = true;
		currentAnimation = &Jump;
		currentVelocity.y = -0.35 * dt;
		pbody->body->SetLinearVelocity(currentVelocity);
		//Movimiento de la camara
		//posar camara vertical quan salta
	}//funciona
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && !dead) {
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !dead) {
		currentVelocity.x = -speed * dt;
		isWalking = true;
		currentAnimation = &Runleft;
		//Movimiento de la cámara
		float camSpeed = 0.2f;
		app->render->camera.x = -(position.x - 100);
	}//funciona

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead) {
		currentVelocity.x = speed * dt;
		isWalking = true;
		currentAnimation = &Runright;
		//Movimiento de la cámara
		float camSpeed = 0.2f;
		app->render->camera.x = -(position.x - 100);
	}//funciona
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
	}//funciona

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && !dead) {
		atacking = true;
		currentAnimation = &Atack1;
		atacktimer = SDL_GetTicks();
	}//No funciona


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