#include "Visualize.hpp"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>

#include "core/Engine.hpp"
#include "core/Handlers.hpp"
#include "module/Log.hpp"


namespace lm {


const std::string Visualize::GAME_NAME("Magneto locator - Visualization");

const std::string Visualize::MODE_NAMES[MODE_COUNT] = {"Visualization"};


using engine::Engine;


// Visualize : ModeHandler
Visualize::Visualize() :
	ModeHandler(DEFAULT_MODE)
{

}

Visualize::~Visualize()
{

}

engine::EventHandler * Visualize::createEventHandler()
{
	if (getInitState() == engine::NOTHING_CREATED) {
		setInitState(engine::EVENT_HANDLER_CREATED);
		return new Events();
	}

	Engine::log << engine::Priority::error
	            << "Trying to create EventHandler without announcing mode change."
	            << std::endl;
	Engine::exit(1);
	return nullptr;
}

engine::ResourceHandler * Visualize::createResourceHandler()
{
	if(getInitState() == engine::EVENT_HANDLER_CREATED) {
		setInitState(engine::RESOURCE_HANDLER_CREATED);
		return new Resources();
	}

	Engine::log << engine::Priority::error
	            << "EventHandler must be created before ResourceHandler."
	            << std::endl;
	Engine::exit(1);
	return nullptr;
}

engine::GameHandler * Visualize::createGameHandler()
{
	if(getInitState() == engine::RESOURCE_HANDLER_CREATED) {
		setInitState(engine::GAME_HANDLER_CREATED);
		return new Logic;
	}

	Engine::log << engine::Priority::error
	            << "ResourceHandler must be created before GameHandler."
	            << std::endl;
	Engine::exit(1);
	return nullptr;
}

const std::string & Visualize::getGameName() const
{
	return GAME_NAME;
}

const std::string & Visualize::getModeName() const
{
	return MODE_NAMES[getMode()];
}

SDL_Rect Visualize::getWindowDimensions() const
{
	return {
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT
	};
}

SDL_Surface * Visualize::getWindowIcon() const
{
	return nullptr;
}

bool Visualize::isValidMode(const Uint16 mode) const
{
	return (mode == 0);
}


// Events : EventHandler
Events::Events()
{
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEWHEEL, SDL_IGNORE);

	SDL_EventState(SDL_TEXTEDITING, SDL_IGNORE);
	SDL_EventState(SDL_TEXTINPUT, SDL_IGNORE);
	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
	SDL_EventState(SDL_KEYUP, SDL_ENABLE);
}

Events::~Events()
{

}

void Events::catchEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		//engine and system events
		if (isEngineCoreEvent(event.type)) {
			handleCoreEvent(event);
			continue;
		}

		//user events follow here
		if (event.type == SDL_KEYDOWN)
			keyboardEvent(event);
	}
}

void Events::keyboardEvent(SDL_Event& event)
{
	if(event.key.state != SDL_PRESSED)
		return;

	Sint32 keySym = event.key.keysym.sym;
	Uint16 keyMod = event.key.keysym.mod;

	switch (keyMod) {
	case KMOD_NONE:
		break;

	case KMOD_LCTRL:    //Ctrl + c
		if(keySym == SDLK_c)
			_engine_.stop();
		return;

	default:
		return;
	}

	switch(keySym) {
	case SDLK_ESCAPE:
		if(_engine_.isRunning())
			_engine_.stop();
		return;
	}

	if(event.key.repeat)
		return;
}


// Logic : GameHandler
Logic::Logic()
{

}

Logic::~Logic()
{

}

void Logic::update()
{

}

void Logic::drawScene(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 128, 128, 255);
	SDL_RenderClear(renderer);
}




} // namespace lm

