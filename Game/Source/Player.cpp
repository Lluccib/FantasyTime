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
	nightidle.LoadAnimations("nightidle", "player");
	nightrun.LoadAnimations("nightrun", "player");
	nightatack.LoadAnimations("nightatack", "player");
	nightdeath.LoadAnimations("nightdeath", "player");
	swordfx = app->audio->LoadFx(parameters.child("swordfx").attribute("path").as_string());
	saltofx = app->audio->LoadFx(parameters.child("saltofx").attribute("path").as_string());
	muertefx = app->audio->LoadFx(parameters.child("muerteplayerfx").attribute("path").as_string());
	checkpointfx = app->audio->LoadFx(parameters.child("checkpointfx").attribute("path").as_string());
	texture = app->tex->Load(texturePath);
	currentAnimation = &idle;

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	level = 1;
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	


	return true;
}

bool Player::Update(float dt)
{
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	

	b2Vec2 currentVelocity = pbody->body->GetLinearVelocity();


	if (life, !isWalking, !jump, !dead, !atacking)
	{
		if (nightborne)
		{
			currentAnimation = &nightidle;
		}
		else
		{
			currentAnimation = &idle;
		}
		

	}	
	if (!life)
	{
		if (nightborne)
		{
			currentAnimation = &nightdeath;
		}
		else
		{
			currentAnimation = &Death;
		}
	}
	

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE )
	{
		isWalking = false;
		
		currentVelocity.x = 0;
		

	}
	if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		nightborne = !nightborne;




	}
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && lives < 5 && curas>0)
	{
		curas--;
		lives++;

	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jump && !dead && !godmode) {
		jump = true;
		app->audio->PlayFx(saltofx);
		currentAnimation = &Jump;
		currentVelocity.y = -0.35 * dt;
		pbody->body->SetLinearVelocity(currentVelocity);
		
	}

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && !jump && !dead && !godmode && !isWalking && !atacking) {
		
		if (nightborne)
		{
			currentAnimation = &nightatack;
		}
		else
		{
			currentAnimation = &Atack1;
		}
		
		if (right)
		{
			atacking = true;
			
			pbody2 = app->physics->CreateRectangleSensor(position.x + 40, position.y + 16, 8, 32, bodyType::STATIC);
			pbody2->ctype = ColliderType::PLAYERATACK;
			pbody2->listener = this;
		}
		else if (left)
		{
			atacking = true;
			
			pbody2 = app->physics->CreateRectangleSensor(position.x - 10, position.y + 16, 8, 32, bodyType::STATIC);
			pbody2->ctype = ColliderType::PLAYERATACK;
			pbody2->listener = this;
		}
		app->audio->PlayFx(swordfx);
		
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
		if (nightborne)
		{
			currentAnimation = &nightrun;
		}
		else
		{
			currentAnimation = &Runright;
		}
		atacking = false;
		float camSpeed = 0.2f;
		right = false;
		left = true;

	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !dead) {
		currentVelocity.x = speed * dt;
		isWalking = true;
		if (nightborne)
		{
			currentAnimation = &nightrun;
		}
		else
		{
			currentAnimation = &Runright;
		}
		atacking = false;
		float camSpeed = 0.2f;
		left = false;
		right = true;
		
	}

	



	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) //START FROM FIRST LEVEL
	{
		cam1 = true;
		level1 = true;
		cam2 = false;
		level2 = false;
		check1 = false;
		check2 = false;
		level = 1;
		pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 4), PIXEL_TO_METERS(32 * 26) }, 0);
		
		
		

	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) //START FROM SECOND LEVEL
	{
		cam1 = false;
		level1 = false;
		cam2 = true;
		level2 = true;
		check1 = false;
		check2 = false;
		level = 2;
		pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 273), PIXEL_TO_METERS(32 * 26) }, 0);



	}

	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		dead = false;
		if (level == 1)
		{
			pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 4), PIXEL_TO_METERS(32 * 26) }, 0);
		}
		else if (level == 2 )
		{
			cam2 = true;
			level2 = true;
			pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 273), PIXEL_TO_METERS(32 * 26) }, 0);
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		dead = false;
		if (!check1 || check2)
		{
			cam1 = true;       
			pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 109), PIXEL_TO_METERS(32 * 26) }, 0);
		}
		else if (check1 &&!check2)
		{
			cam2 = true;
			cam1 = false;
			level1 = false;
			level2 = true;
			pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 362), PIXEL_TO_METERS(32 * 12) }, 0);
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
		currentAnimation->loopCount = 0;

	}
	else if (currentAnimation == &Atack1left || currentAnimation == &nightatack && currentAnimation->HasFinished())
	{

		atacking = false;
		destroybody = true;
		currentAnimation->Reset();
		currentAnimation->loopCount = 0;


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
			if (lives >= 0)
			{
				if (!check1 || !check2 || !level2)
				{
					pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 4), PIXEL_TO_METERS(32 * 26) }, 0);
				}
				else if (check1)
				{
					pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 109), PIXEL_TO_METERS(32 * 26) }, 0);
				}
				else if (check2)
				{
					pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 362), PIXEL_TO_METERS(32 * 12) }, 0);
				}
				else if (level2)
				{
					pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 273), PIXEL_TO_METERS(32 * 26) }, 0);
				}
				dead = false;
				life = true;
				nightborne = false;
				

				currentAnimation->Reset();
			}
			
			

		}
	}
	

	

	pbody->body->SetLinearVelocity(currentVelocity);
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	if (!nightborne)
	{
		speed = 0.2f;
		damage = 1;
		speedy = 0.5f;
		if (left)
		{
			app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
			currentAnimation->Update();
		}
		if (right)
		{
			app->render->DrawTexture(texture, position.x - 5, position.y, &currentAnimation->GetCurrentFrame());
			currentAnimation->Update();
		}
	}
	else if (nightborne)
	{
		speed = 0.3f;
		damage = 3;
		speedy = 0.7f;
		if (left)
		{
			app->render->DrawTexture(texture, position.x - 30, position.y - 30, &currentAnimation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
			currentAnimation->Update();
		}
		if (right)
		{
			app->render->DrawTexture(texture, position.x - 30, position.y - 30, &currentAnimation->GetCurrentFrame());
			currentAnimation->Update();
		}
	}
	
	//Movimiento de la camara, y bloqueo de la camara
	if (cam1)
	{
		if (app->render->camera.x - position.x - 100 <= -200 && app->render->camera.x - position.x - 100 >= -12850) {
			app->render->camera.x = -(position.x - 100);

		}

		if (app->render->camera.x - position.x - 100 <= -12900) {
			app->render->camera.x = -6333;
		}
	}
	else if (cam2)
	{
		if (app->render->camera.x - position.x - 100 <= -200 && app->render->camera.x - position.x - 100 >= -30000) {
			app->render->camera.x = -(position.x - 100);

		}

		if (app->render->camera.x - position.x - 100 <= -30000) {
			app->render->camera.x = -6333;
		}
	}
	
	
	printf("\r %i", app->render->camera.x);
	if (tp2)
	{
		pbody->body->SetTransform({ PIXEL_TO_METERS(32 * 273), PIXEL_TO_METERS(32 * 26) }, 0);
		tp2 = false;
	}

	

	
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
				lives--;
				deathtimer = SDL_GetTicks();
				LOG("Collision PLATFORM");
				break;
			case ColliderType::UNKNOWN:
				LOG("Collision UNKNOWN");
				break;
			case ColliderType::ENEMYATTACK:
				life = false;
				dead = true;
				lives--;
				deathtimer = SDL_GetTicks();
				LOG("Collision PLATFORM");
				break;
			case ColliderType::CHECKPOINT:
				if (!check1)
				{
					app->audio->PlayFx(checkpointfx);
					check1 = true;
					check2 = false;
					level1 = false;
					level2 = false;
					LOG("Collision CHECKPONT");
					
				}
				
				LOG("Collision CHECKPONT");
				/*app->audio->PlayFx(pickCoinFxId);*/
				break;
			case ColliderType::CHECKPOINT2:
				if (!check2)
				{
					app->audio->PlayFx(checkpointfx);
					check1 = false;
					check2 = true;
					level1 = false;
					level2 = false;
					LOG("Collision CHECKPONT");
					
				}
				LOG("Collision CHECKPONT");
				/*app->audio->PlayFx(pickCoinFxId);*/
				break;
			case ColliderType::LEVELEND:
				
					
					check1 = false;
					check2 = false;
					level1 = false;
					level2 = true;
					cam1 = false;
					cam2 = true;
					tp2 = true;
					//c
				LOG("Collision CHECKPONT");
				/*app->audio->PlayFx(pickCoinFxId);*/
				break;
			case ColliderType::COIN:
				LOG("Collision ITEM");
				app->audio->PlayFx(pickCoinFxId);
				break;
			case ColliderType::HEAL:
				LOG("Collision ITEM");
				app->audio->PlayFx(pickCoinFxId);
				if (curas < 5)
				{
					curas++;
				}
				
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
	
	
	
}