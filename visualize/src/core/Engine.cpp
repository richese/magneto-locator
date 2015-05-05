#include "Engine.hpp"

#include <cstdio>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


namespace engine {


Engine* Engine::_singleton_ = nullptr;

Log Engine::log("log.txt");


Engine & Engine::create(ModeHandler* gameModes)
{
	if (_singleton_ != nullptr) {
		log << Priority::error << "Trying to recreate engine." << std::endl;
		return (*_singleton_);
	}

	_singleton_ = new Engine(gameModes);
	return (*_singleton_);
}

Engine & Engine::get()
{
	if (_singleton_ == nullptr) {
		log << Priority::critical << "Engine was not created yet." << std::endl;
		Engine::exit(1);
	}

	return (*_singleton_);
}


bool Engine::wasCreated()
{
	return (_singleton_ != nullptr);
}

void Engine::exit(int status)
{
	if (_singleton_ != nullptr)
		delete _singleton_;

	if (TTF_WasInit())
		TTF_Quit();

	IMG_Quit();

	if (SDL_WasInit(0))
		SDL_Quit();

	std::exit(status);
}

void Engine::PushGameModeChange(const Uint16 mode)
{
	Uint16* temp = new Uint16;
	*temp = mode;

	SDL_Event event;

	event.type = _singleton_->ENGINE_CORE_EVENT;
	//event.user.type = ENGINE_CORE_EVENT;

	event.user.code = CHANGE_GAME_MODE_EVENT;
	event.user.data1 = temp;
	event.user.data2 = nullptr;

	SDL_PushEvent(&event);
}

void Engine::PushEngineStop()
{
	SDL_Event event;

	event.type = _singleton_->ENGINE_CORE_EVENT;
	//event.user.type = ENGINE_CORE_EVENT;

	event.user.code = STOP_ENGINE_EVENT;
	event.user.data1 = nullptr;
	event.user.data2 = nullptr;

	SDL_PushEvent(&event);
}

void Engine::SDL_LogRedirectFunction(void *userdata, int category,
                                     SDL_LogPriority priority,
                                     const char *message)
{
	switch (priority) {
	default:
	case SDL_LOG_PRIORITY_VERBOSE:
		log << Priority::verbose << message << std::endl;
		break;
	case SDL_LOG_PRIORITY_DEBUG:
		log << Priority::debug << message << std::endl;
		break;
	case SDL_LOG_PRIORITY_INFO:
		log << Priority::info << message << std::endl;
		break;
	case SDL_LOG_PRIORITY_WARN:
		log << Priority::warn << message << std::endl;
		break;
	case SDL_LOG_PRIORITY_ERROR:
		log << Priority::error << message << std::endl;
		break;
	case SDL_LOG_PRIORITY_CRITICAL:
		log << Priority::critical << message << std::endl;
		break;
	}
}

Engine::Engine(ModeHandler* gameModes) :
	ENGINE_CORE_EVENT(registerCoreEvents()),
	initializedMode_(false),
	running_(false),
	paused_(false),
	window_(nullptr),
	modes_(gameModes),
	events_(nullptr),
	resources_(nullptr),
	game_(nullptr)
{
	log.currentTime();

	//initialize required parts of SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0) {
		log << Priority::critical << "Unable to initialize SDL2 library."
		    << std::endl << SDL_GetError() << std::endl;
		Engine::exit(1);
	}

	//redirect SDL loging
	SDL_LogSetOutputFunction(SDL_LogRedirectFunction, nullptr);
	SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

	//log SDL lib version
	log << Priority::verbose << "Compile time SDL2 version: "
	    << SDL_MAJOR_VERSION << '.' << SDL_MINOR_VERSION << '.'
	    << SDL_PATCHLEVEL << std::endl;

	//load SDL_Image
	if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
		log << Priority::error << "Failed to initialize SDL2_image library."
		    << std::endl << IMG_GetError() << std::endl;
		Engine::exit(1);
	}

	//log SDL_Image version
	log << Priority::verbose << "Compile time SDL2_image version: "
	    << SDL_IMAGE_MAJOR_VERSION << '.' << SDL_IMAGE_MINOR_VERSION
	    << '.' << SDL_IMAGE_PATCHLEVEL << std::endl;

	//load SDL_ttf
	if (TTF_Init() == -1) {
		log << Priority::error << "Failed to initialize SDL2_ttf library."
		    << std::endl << TTF_GetError() << std::endl;
		Engine::exit(1);
	}

	//log SDL_ttf version
	log << Priority::verbose << "Compile time SDL2_ttf version: "
	    << TTF_MAJOR_VERSION << '.' << TTF_MINOR_VERSION << '.'
	    << TTF_PATCHLEVEL << std::endl;

	if (modes_ == nullptr) {
		log << Priority::critical << "No game modes defined."
		    << std::endl;
		Engine::exit(1);
	}

	//WindowHandler initialization
	window_ = new WindowHandler(false, true, false);
	if (!window_->initialize(modes_->getGameName(),
	                         modes_->getWindowDimensions(),
	                         nullptr) ) {
		Engine::exit(1);
	}

	log << Priority::info << "Engine core initialized." << std::endl;

	_singleton_ = this;
	this->initializeMode();
}

Engine::~Engine()
{
	if (game_ != nullptr)
		delete game_;

	if (resources_ != nullptr)
		delete resources_;

	if (events_ != nullptr)
		delete events_;

	if (window_ != nullptr)
		delete window_;

	if (modes_ != nullptr)
		delete modes_;

	log << Priority::info << "Shuting down SDL components." << std::endl;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	log << Priority::verbose << "Engine deleted." << std::endl;
	log.currentTime();
}

void Engine::initializeMode()
{
	initializedMode_ = false;

	//EventHandler initialization
	if (events_ != nullptr)
		delete events_;
	events_ = modes_->createEventHandler();

	//ResourceHandler initialization
	if (resources_ != nullptr && !resources_->isValid(modes_->getMode()) )
		delete resources_;
	resources_ = modes_->createResourceHandler();

	//GameHandler initialization
	if (game_ != nullptr)
		delete game_;
	game_ = modes_->createGameHandler();


	initializedMode_ = true;

	log << Priority::info << "Game mode " << modes_->getModeName()
	    << " was initialized." << std::endl;
}

void Engine::start()
{
	if (!running_) {
		if (!initializedMode_) {
			log << Priority::error << "Game mode was not initialized yet."
			    << std::endl;
			return;
		}

		paused_ = false;

		log << Priority::info << "Engine started." << std::endl;
		SDL_SetRenderDrawColor(window_->getRenderer(),
		                       255, 255, 255, 255);
		SDL_RenderClear(window_->getRenderer());
	} else {
		if (paused_)
			paused_ = false;
		else
			log << Priority::warn << "Engine is already running."
			    << std::endl;
		return;
	}

	SDL_Renderer* renderer = window_->getRenderer();

	running_ = true;
	while (running_) {
		if (modes_->changingMode())
			initializeMode();

		events_->catchEvents();

		if (paused_ || !running_)
			continue;

		game_->update();

		//sem nahadz co vykreslit
		if (window_->hasToRedraw()) {
			window_->setRedrawFlag(false);
			game_->drawScene(renderer);
			SDL_RenderPresent(renderer);
		}
	}
}

void Engine::pause()
{
	if (running_) {
		paused_ = true;
	} else {
		log << Priority::warn << "Trying to pause engine that is not running."
		    << std::endl;
	}
}

void Engine::stop()
{
	if (running_) {
		log << Priority::info << "Engine stopped." << std::endl;
		running_ = false;
	}
	else {
		log << Priority::warn << "Trying to stop engine that is not running."
		    << std::endl;
	}
}

bool Engine::isPaused() const
{
	return paused_;
}

bool Engine::isRunning() const
{
	return running_;
}

WindowHandler & Engine::getWindow()
{
	if (window_ == nullptr) {
		log << Priority::error << "Invalid WindowHandler!" << std::endl;
		Engine::exit(1);
	}

	return (*window_);
}

ModeHandler & Engine::getModes()
{
	if (modes_ == nullptr) {
		log << Priority::error << "Invalid ModeHandler!" << std::endl;
		Engine::exit(1);
	}

	return (*modes_);
}

EventHandler & Engine::getEvents()
{
	if (events_ == nullptr) {
		log << Priority::error << "Invalid EventHandler!!!" << std::endl;
		Engine::exit(1);
	}

	return (*events_);
}

ResourceHandler & Engine::getResources()
{
	if (resources_ == nullptr) {
		log << Priority::error << "Invalid ResourceHandler!!!" << std::endl;
		Engine::exit(1);
	}

	return (*resources_);
}

GameHandler & Engine::getGameHandler()
{
	if (game_ == nullptr) {
		log << Priority::error << "Invlaid GameHandler!!!" << std::endl;
		Engine::exit(1);
	}

	return (*game_);
}

Uint32 Engine::registerCoreEvents()
{
	Uint32 eventTypeId = SDL_RegisterEvents(1);

	log << Priority::debug << "Core events type id: " << eventTypeId
	    << std::endl;

	if (eventTypeId != SDL_USEREVENT ) {
		log << Priority::critical << "Core events id is already reserved."
		    << std::endl;
		Engine::exit(2);
	}

	return eventTypeId;
}


} // namespace engine
