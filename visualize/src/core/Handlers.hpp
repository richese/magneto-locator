#ifndef _SDL2_ENGINE_HANDLERS_H_
#define _SDL2_ENGINE_HANDLERS_H_

#include <string>

#include <SDL2/SDL.h>

//#include "Engine.hpp"

// forward declaration of Engine
namespace engine {
class Engine;
} // namespace engine


namespace engine {


class EventHandler
{
public:
	Engine &_engine_;

public:
	EventHandler();
	virtual ~EventHandler();


	virtual void catchEvents() = 0;

	bool isBusy() const;

	void stopEventHandling();

protected:
	virtual void keyboardEvent(SDL_Event& event) = 0;

	bool isEngineCoreEvent(const Uint32 eventType);

	void handleCoreEvent(SDL_Event& event);

private:
	void handleWindowEvent(const Uint8 id); //SDL_WindowEvent.event

};


class GameHandler
{
public:
	GameHandler();
	virtual ~GameHandler();

	virtual void update() = 0;
	virtual void drawScene(SDL_Renderer* renderer) = 0;
};


class ResourceHandler
{
public:
	ResourceHandler();
	virtual ~ResourceHandler();

	virtual bool isValid(const Uint16 mode) = 0;
};


typedef enum ModeInitState {
	NOTHING_CREATED          = 0,
	EVENT_HANDLER_CREATED    = 1,
	RESOURCE_HANDLER_CREATED = 2,
	GAME_HANDLER_CREATED     = 3
} ModeInitState;


class ModeHandler
{
	Uint16 currentMode_;
	Uint8  modeInitState_;

	const static std::string dummyName;
	const static std::string dummyMode;

public:
	ModeHandler(const Uint16 mode);
	virtual ~ModeHandler();


	virtual EventHandler * createEventHandler() = 0;
	virtual ResourceHandler * createResourceHandler() = 0;
	virtual GameHandler * createGameHandler() = 0;

	bool setMode(const Uint16 mode);
	Uint16 getMode() const;
	bool changingMode() const;


	virtual const std::string& getModeName() const = 0;
	virtual const std::string& getGameName() const = 0;
	virtual SDL_Rect getWindowDimensions() const = 0;
	virtual SDL_Surface* getWindowIcon() const;

protected:
	virtual bool isValidMode(const Uint16 mode) const = 0;
	void setInitState(const ModeInitState state);
	Uint8 getInitState() const;
};


} // namespace engine


#endif // _SDL2_ENGINE_HANDLERS_H_
