#include "Visualize.hpp"

#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "core/Engine.hpp"
#include "core/Handlers.hpp"
#include "module/Log.hpp"


namespace lm {


const std::string Visualize::GAME_NAME("Magneto locator - Visualization");

const std::string Visualize::MODE_NAMES[MODE_COUNT] = {"Visualization"};

using engine::Engine;
using engine::Priority;


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

//
// Resources
//
Resources::Resources() :
	shared_output_(),
	magneto_data_()
{
	if (shared_output_.init() != 0) {
		Engine::exit(-1);
	}
	shared_output_.set_visualize_state(CONN_ACTIVE);
}

Resources::~Resources()
{
	shared_output_.set_visualize_state(CONN_NONE);
	shared_output_.unlink();
}

int Resources::updateData()
{
	if (shared_output_.is_process_connected()) {
		MagnetoData tmp = shared_output_.get_data();
		if (tmp.timestamp > magneto_data_.timestamp) {
			magneto_data_ = tmp;
			return 0;
		}
		return 1;
	}

	return -1;
}

const MagnetoData & Resources::getData() const
{
	return magneto_data_;
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
	case SDLK_q:
		if(_engine_.isRunning())
			_engine_.stop();
		return;
	}

	if(event.key.repeat)
		return;
}


// Logic : GameHandler
Logic::Logic() :
	timerId_(0),
	timerInterval_(100),
	updateRequest_(true),
	scale_(1.0),
	shift_(),
	draw_axis_(true),
	draw_sensors_(true),
	draw_circles_(true),
	draw_poi_(true),
	draw_result_(true)
{
	//
	// Setup update timer.
	//
	timerId_ = SDL_AddTimer(timerInterval_, updateTimerCallback, this);
	if (timerId_ == 0) {
		Engine::log << engine::Priority::error
		            << "Unable to create update timer"
		            << std::endl;
		Engine::exit(-1);
	}
	Engine::log << Priority::debug
	            << "Started timer: " << timerId_ << std::endl;

	//
	// Set coordinate system.
	//
	SDL_Rect dim = Engine::get().getModes().getWindowDimensions();
	shift_.x = dim.w / 2;
	shift_.y = dim.h / 2;
	scale_ = dim.w / 120.0;
	Engine::log << Priority::debug << "Scale: " << scale_ << std::endl;
	Engine::log << Priority::debug << "Shift: " << shift_ << std::endl;
}

Logic::~Logic()
{
	SDL_RemoveTimer(timerId_);
	Engine::log << Priority::debug
	            << "Stopped timer: " << timerId_ << std::endl;
}

void Logic::update()
{
	if (updateRequest_) {
		Resources &res = (Resources &) Engine::get().getResources();
		res.updateData();
		updateRequest_ = false;
		Engine::get().getWindow().setRedrawFlag(true);
	}
}

void Logic::drawScene(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);

	Resources &res = (Resources &) Engine::get().getResources();
	const MagnetoData &data = res.getData();
	if (not data.valid)
		return;

	//
	// Draw axis.
	//
	if (draw_axis_)
		drawAxis(renderer, 10.0, 0x60000000);

	//
	// Draw sensor positions.
	//
	if (draw_sensors_) {
		for (int i = 0; i < MD_SENSOR_COUNT; ++i)
			drawSensor(renderer, data.sensor[i], 0xFF000000);
	}

	//
	// Draw circles.
	//
	if (draw_circles_) {
		for (int i = 0; i < MD_CIRCLE_COUNT; ++i)
			drawCircle(renderer, data.circle[i], 0xFF0F0F0F);
	}

	//
	// Draw poi.
	//
	if (draw_poi_)
		drawPoint(renderer, data.poi, 0xFFFF0000);

	//
	// Draw result.
	//
	if (draw_result_)
		drawPoint(renderer, data.result.mirror_y(), 0xFF0000FF);

}

void Logic::drawAxis(SDL_Renderer *renderer, double interval, Uint32 color)
{
	if (interval < 0.0)
		interval = -interval;
	interval *= scale_;

	SDL_Rect window = Engine::get().getModes().getWindowDimensions();

	hlineColor(renderer,
	           0, window.w,
	           window.h / 2.0,
	           color);
	vlineColor(renderer,
		   window.w / 2.0,
		   0, window.h,
		   color);

	const double kSize = 5;
	//os x
	for (double w = window.w / 2.0; w <= window.w; w += interval) {
		vlineColor(renderer,
			   w,
		           (window.h / 2.0) - kSize,
		           (window.h / 2.0) + kSize,
		           color);
	}
	for (double w = window.w / 2.0; w >= 0.0; w -= interval) {
		vlineColor(renderer,
			   w,
		           (window.h / 2.0) - kSize,
		           (window.h / 2.0) + kSize,
		           color);
	}
	// os y
	for (double h = window.h / 2.0; h <= window.h; h += interval) {
		hlineColor(renderer,
		           (window.w / 2.0) - kSize,
		           (window.w / 2.0) + kSize,
		           h,
		           color);
	}
	for (double h = window.h / 2.0; h >= 0; h -= interval) {
		hlineColor(renderer,
		           (window.w / 2.0) - kSize,
		           (window.w / 2.0) + kSize,
		           h,
		           color);
	}
}

void Logic::drawPoint(SDL_Renderer *renderer, Point pos, Uint32 color)
{
	const Sint16 size = scale_ * 1.0;

	pos = (scale_ * (-pos)) + shift_;

	hlineColor(renderer,
	           pos.x - size, pos.x + size,
	           pos.y,
		   color);
	vlineColor(renderer,
	           pos.x,
	           pos.y + size, pos.y - size,
		   color);
}

void Logic::drawSensor(SDL_Renderer *renderer, Point pos, Uint32 color)
{
	pos = (scale_ * pos.mirror_y()) + shift_;

	//rectangleColor(renderer,
	               //pos.x - (scale_ * 0.8), pos.y + (scale_ * 1.0),
	               //pos.x + (scale_ * 0.8), pos.y - (scale_ * 0.8),
	               //color);
	boxColor(renderer,
	         pos.x - (scale_ * 0.8), pos.y + (scale_ * 1.0),
	         pos.x + (scale_ * 0.8), pos.y - (scale_ * 0.8),
	         color);
}

void Logic::drawCircle(SDL_Renderer *renderer, Circle circ, Uint32 color)
{
	Point center = (scale_ * circ.center().mirror_x()) + shift_;

	circleColor(renderer,
	            center.x, center.y,
	            scale_ * circ.radius(),
	            color);
}




Uint32 updateTimerCallback(Uint32 interval, void *param)
{
	Logic *logic = (Logic*) param;
	logic->updateRequest();
	return interval;
}





} // namespace lm

