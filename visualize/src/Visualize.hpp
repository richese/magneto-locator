#ifndef _MAGNETO_VISUALIZE_VISUALIZE_H_
#define _MAGNETO_VISUALIZE_VISUALIZE_H_

#include <string>

#include <SDL2/SDL.h>

#include "core/Handlers.hpp"


namespace lm {

const int MODE_COUNT = 1;


class Visualize : public engine::ModeHandler
{
public:
	static const Uint16 DEFAULT_MODE = 0;
	static const std::string GAME_NAME;
	static const std::string MODE_NAMES[];
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HEIGHT = 720;

public:
	Visualize();
	~Visualize();

	engine::EventHandler* createEventHandler();
	engine::ResourceHandler* createResourceHandler();
	engine::GameHandler* createGameHandler();

	const std::string& getModeName() const;
	const std::string& getGameName() const;

	SDL_Rect getWindowDimensions() const;
	SDL_Surface* getWindowIcon() const;

protected:
	bool isValidMode(const Uint16 mode) const;
};


class Resources : public engine::ResourceHandler
{
public:
	bool isValid(const Uint16 mode) { return (mode == 0); };
};


class Events : public engine::EventHandler
{
public:
	Events();
	~Events();

	void catchEvents();

private:
	void keyboardEvent(SDL_Event& event);
};


class Logic :public engine::GameHandler
{
public:
	Logic();
	~Logic();

	void update();
	void drawScene(SDL_Renderer* renderer);
};


} // namespace lm


#endif // _MAGNETO_VISUALIZE_VISUALIZE_H_
