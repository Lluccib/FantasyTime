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

	//initilize textures
	//idle
	idle.PushBack({ 47, 16, 30, 49 });
	idle.PushBack({ 175, 16, 30, 49 });
	idle.PushBack({ 47, 80, 30, 49 });
	idle.PushBack({ 175, 80, 30, 49 });
	idle.PushBack({ 47, 144, 30, 49 });
	idle.PushBack({ 175, 144, 30, 49 });
	idle.PushBack({ 47, 208, 30, 49 });
	idle.PushBack({ 175, 208, 30, 49 });
	idle.speed = 0.1f;
	idle.loop = true;

	//right run
	Runright.PushBack({ 345, 17, 32, 49 });
	Runright.PushBack({ 473, 17, 30, 49 });
	Runright.PushBack({ 345, 80, 30, 49 });
	Runright.PushBack({ 473, 80, 30, 49 });
	Runright.PushBack({ 345, 145, 30, 49 });
	Runright.PushBack({ 471, 145, 30, 49 });
	Runright.PushBack({ 345, 209, 30, 49 });
	Runright.PushBack({ 473, 209, 30, 49 });
	Runright.speed = 0.2f;
	Runright.loop = true;
	//left run
	Runleft.PushBack({ 1430, 24, 32, 49 });
	Runleft.PushBack({ 1558, 24, 30, 49 });
	Runleft.PushBack({ 1430, 88, 30, 49 });
	Runleft.PushBack({ 1558, 88, 30, 49 });
	Runleft.PushBack({ 1430, 153, 30, 49 });
	Runleft.PushBack({ 1558, 153, 30, 49 });
	Runleft.PushBack({ 1430, 217, 30, 49 });
	Runleft.PushBack({ 1558, 217, 30, 49 });
	Runleft.speed = 0.2f;
	Runleft.loop = true;
	//pray
	Pray.PushBack({ 939, 19, 32, 49 });
	Pray.PushBack({ 1065, 19, 30, 49 });
	Pray.PushBack({ 1194, 19, 30, 49 });
	Pray.PushBack({ 1323, 19, 30, 49 });
	
	Pray.speed = 0.05f;
	Pray.loop = false;


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
	
	if (!isWalking, !jump)
	{
		currentAnimation = &idle;
	}

	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
		isWalking = false;
		currentVelocity.x = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && !jump) {
		jump = true;
		currentVelocity.y = -0.3 * dt;
		pbody->body->SetLinearVelocity(currentVelocity);

	}
	
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		currentVelocity.x = -speed * dt;
		currentAnimation = &Runleft;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		currentVelocity.x = speed * dt;
		isWalking = true;
		currentAnimation = &Runright;
	}
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		
			
			currentAnimation = &Pray;
		
		
	}
	//Set the velocity of the pbody of the player
	if (jump == false) {
		currentVelocity.y = -GRAVITY_Y	;

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