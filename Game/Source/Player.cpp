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
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{
	

	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
	{
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

	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		currentVelocity.x = speed * dt;
	}

	//Set the velocity of the pbody of the player
	if (jump == false) {
		currentVelocity.y = -GRAVITY_Y;

	}
	pbody->body->SetLinearVelocity(currentVelocity);
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

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
		app->audio->PlayFx(pickCoinFxId);
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