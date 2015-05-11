#ifndef _MAGNETO_VISUALIZE_VISUALIZE_H_
#define _MAGNETO_VISUALIZE_VISUALIZE_H_

#include <string>

#include <SDL2/SDL.h>

#include "core/Handlers.hpp"
#include "shared.hpp"


namespace lm {

const int MODE_COUNT = 1;

//------------------------------------------------------------------------------
//
// Visualize
//
//------------------------------------------------------------------------------
class Visualize : public engine::ModeHandler
{
public:
	static const Uint16 DEFAULT_MODE = 0;
	static const std::string GAME_NAME;
	static const std::string MODE_NAMES[];
	static const int WINDOW_WIDTH = 900;
	static const int WINDOW_HEIGHT = 700;

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


//------------------------------------------------------------------------------
//
// Resources
//
//------------------------------------------------------------------------------
class Resources : public engine::ResourceHandler
{
	Shared shared_output_;
	MagnetoData magneto_data_;

public:
	Resources();
	~Resources();

	bool isValid(const Uint16 mode) { return (mode == 0); };

	int updateData();
	const MagnetoData & getData() const;
};


//------------------------------------------------------------------------------
//
// Events
//
//------------------------------------------------------------------------------
class Events : public engine::EventHandler
{
public:
	Events();
	~Events();

	void catchEvents();

private:
	void keyboardEvent(SDL_Event& event);
};


//------------------------------------------------------------------------------
//
// Logic
//
//------------------------------------------------------------------------------
class Logic :public engine::GameHandler
{
	SDL_TimerID timerId_;
	Uint32 timerInterval_;
	bool updateRequest_;

	double scale_;
	Point shift_;

	bool draw_axis_;
	bool draw_sensors_;
	bool draw_circles_;
	bool draw_poi_;
	bool draw_result_;

public:
	Logic();
	~Logic();

	void update();
	void drawScene(SDL_Renderer *renderer);

	inline void updateRequest() { updateRequest_ = true; }

private:
	void drawAxis(SDL_Renderer *renderer, double interval, Uint32 color);
	void drawPoint(SDL_Renderer *renderer, Point pos, Uint32 color);
	void drawSensor(SDL_Renderer *renderer, Point pos, Uint32 color);
	void drawCircle(SDL_Renderer *renderer, Circle circ, Uint32 color);
};


Uint32 updateTimerCallback(Uint32 interval, void *param);


} // namespace lm


#endif // _MAGNETO_VISUALIZE_VISUALIZE_H_
