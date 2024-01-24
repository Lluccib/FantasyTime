#include "ModuleFadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Window.h"

#include "SDL/include/SDL_render.h"

ModuleFadeToBlack::ModuleFadeToBlack(App* app, bool start_enabled) : Module(app, start_enabled) {
	screenRect = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
}

ModuleFadeToBlack::~ModuleFadeToBlack() {
}

bool ModuleFadeToBlack::Start() {
	LOG("Preparing Fade Screen");

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool ModuleFadeToBlack::Update(float dt) {
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return update_status::UPDATE_CONTINUE;

	if (currentStep == Fade_Step::TO_BLACK) {
		++frameCount;
		if (frameCount >= maxFadeFrames) {
			moduleToDisable->Disable();
			moduleToEnable->Enable();

			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else {
		--frameCount;
		if (frameCount <= 0) currentStep = Fade_Step::NONE;
	}

	return update_status::UPDATE_CONTINUE;
}

bool ModuleFadeToBlack::PostUpdate() {
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return update_status::UPDATE_CONTINUE;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return update_status::UPDATE_CONTINUE;
}

bool ModuleFadeToBlack::FadeToBlack(Module* moduleToDisable, Module* moduleToEnable, float frames) {
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE) {
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = static_cast<Uint32>(frames);

		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}
