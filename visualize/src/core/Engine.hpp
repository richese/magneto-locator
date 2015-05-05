#ifndef _SDL2_ENGINE_ENGINE_H_
#define _SDL2_ENGINE_ENGINE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "module/Log.hpp"
#include "Handlers.hpp"
#include "Window.hpp"


#define HW_ACCELERATED_WINDOW 1


namespace engine {


class Engine
{
// static
	static Engine*      _singleton_;

public:
	static Log  log;
	const Uint32 ENGINE_CORE_EVENT;

public:
	static Engine& create(ModeHandler* gameModes);
	static Engine& get();
	static bool wasCreated();
	static void exit(int status);

	static void PushGameModeChange(const Uint16 mode);
	static void PushEngineStop();

private:
	static void SDL_LogRedirectFunction(void *userdata, int category,
	                                    SDL_LogPriority priority,
	                                    const char *message);
//
private:
	bool initializedMode_;
	bool running_;
	bool paused_;


	WindowHandler *window_;
	ModeHandler *modes_;
	EventHandler *events_;
	ResourceHandler *resources_;
	GameHandler *game_;

public:
	~Engine();
	void initializeMode();

	void start();
	void pause();
	void stop();
	bool isPaused() const;
	bool isRunning() const;

	WindowHandler & getWindow();
	ModeHandler & getModes();
	EventHandler & getEvents();
	ResourceHandler & getResources();
	GameHandler & getGameHandler();


private:
	Engine(ModeHandler* gameModes);
	Uint32 registerCoreEvents();
};


enum _EventCode {
	UNUSED                  = 0,
	CHANGE_GAME_MODE_EVENT  = 1,
	STOP_ENGINE_EVENT       = 2
};


} // namespace engine


#endif // _SDL2_ENGINE_ENGINE_H_
