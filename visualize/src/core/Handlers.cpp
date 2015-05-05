#include "Handlers.hpp"

#include <iostream>
#include <string>

#include "Engine.hpp"
#include "Window.hpp"

namespace engine {


const std::string ModeHandler::dummyName("Dummy game.");
const std::string ModeHandler::dummyMode("Dummy mode.");


EventHandler::EventHandler() :
	_engine_(Engine::get())
{

}

EventHandler::~EventHandler()
{

}

void EventHandler::catchEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		//engine and system events
		if (isEngineCoreEvent(event.type)) {
			handleCoreEvent(event);
			continue;
		}

		//user events follow here
	}

}

bool EventHandler::isEngineCoreEvent(const Uint32 eventType)
{
	return (eventType == _engine_.ENGINE_CORE_EVENT ||
	        eventType == SDL_QUIT ||
	        eventType == SDL_WINDOWEVENT);
}

void EventHandler::handleCoreEvent(SDL_Event& event)
{
	switch (event.type) {
	case SDL_QUIT:
		if (_engine_.isRunning())
			_engine_.stop();
		return;

	case SDL_WINDOWEVENT:
		handleWindowEvent(event.window.event);
		return;

	case 0x8000:
		switch (event.user.code) {
		case CHANGE_GAME_MODE_EVENT:
			{
				Uint16 mode =  *(Uint16*) event.user.data1;
				delete (Uint16*) event.user.data1;

				if (!_engine_.getModes().setMode(mode))
					Engine::exit(1);
			}
			return;

		case STOP_ENGINE_EVENT:
			if (_engine_.isRunning())
				_engine_.stop();
			return;
		}
		return;
	}
}

void EventHandler::handleWindowEvent(const Uint8 id)
{
	WindowHandler& window = _engine_.getWindow();

	switch (id) {
	case SDL_WINDOWEVENT_SHOWN:
	case SDL_WINDOWEVENT_EXPOSED:
		window.setRedrawFlag(true);
		break;

	case SDL_WINDOWEVENT_SIZE_CHANGED:
	case SDL_WINDOWEVENT_RESIZED:
		///TODO: Zmena velkosti okna
		window.setRedrawFlag(true);
		break;

	case SDL_WINDOWEVENT_FOCUS_LOST:
	case SDL_WINDOWEVENT_MINIMIZED:
		_engine_.pause();
		break;

	case SDL_WINDOWEVENT_FOCUS_GAINED:
	case SDL_WINDOWEVENT_MAXIMIZED:
	case SDL_WINDOWEVENT_RESTORED:
		if (_engine_.isPaused())
			_engine_.start();
		window.setRedrawFlag(true);
		break;

	case SDL_WINDOWEVENT_CLOSE:
		if (_engine_.isRunning())
			_engine_.stop();
		break;
	}
}

// GameHandler
GameHandler::GameHandler()
{

}

GameHandler::~GameHandler()
{

}

void GameHandler::update()
{
	//nothing to do
}

void GameHandler::drawScene(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 138, 128, 128, 255);
    SDL_RenderClear(renderer);
}

// ResourceHandler
ResourceHandler::ResourceHandler()
{
    //nothing to do
}

ResourceHandler::~ResourceHandler()
{
    //nothng to do
}

bool ResourceHandler::isValid(const Uint16 mode)
{
    return false;
}

// ModeHandler
ModeHandler::ModeHandler(const Uint16 mode)
{
	currentMode_   = mode;
	modeInitState_ = NOTHING_CREATED;
}

ModeHandler::~ModeHandler()
{

}

bool ModeHandler::setMode(const Uint16 mode)
{
	if (isValidMode(mode)) {
		currentMode_ = mode;
		modeInitState_ = NOTHING_CREATED;

		Engine::log << Priority::verbose
		            << "Ready to change game mode to mode: "
		            << getModeName() << std::endl;
		return true;
	}

	Engine::log << Priority::error
	            << "Trying to switch to illegal game mode: "
	            << mode << std::endl;
	return false;
}

Uint16 ModeHandler::getMode() const
{
	return currentMode_;
}


bool ModeHandler::changingMode() const
{
	return (modeInitState_ != GAME_HANDLER_CREATED) ? true : false;
}

const std::string & ModeHandler::getModeName() const
{
	return dummyMode;
}

const std::string& ModeHandler::getGameName() const
{
	return dummyName;
}

SDL_Rect ModeHandler::getWindowDimensions() const
{
	return { 0, 0, 0, 0};
}

SDL_Surface * ModeHandler::getWindowIcon() const
{
	return nullptr;
}

void ModeHandler::setInitState(const ModeInitState state)
{
	modeInitState_ = state;
}

Uint8 ModeHandler::getInitState() const
{
	return modeInitState_;
}


} // namespace engine
