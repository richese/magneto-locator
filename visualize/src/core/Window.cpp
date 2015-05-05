#include "Window.hpp"

#include <SDL2/SDL.h>

#include "Engine.hpp"


namespace engine {


WindowHandler::WindowHandler() :
	WindowHandler(true, true, false)
{

}

WindowHandler::WindowHandler(const bool resizable, const bool cursor,
                             const bool fullscreen) :
	initialized_    (false),
	resizable_      (resizable),
	showCursor_     (cursor),
	fullscreen_     (fullscreen),
	window_         (nullptr),
	renderer_       (nullptr),
	windowFlags_    (SDL_WINDOW_SHOWN),
	rendererFlags_  (0)
{
	if (resizable_)
		windowFlags_ |= SDL_WINDOW_RESIZABLE;

#ifdef HW_ACCELERATED_WINDOW
	rendererFlags_ |= SDL_RENDERER_ACCELERATED;
#endif
}

WindowHandler::~WindowHandler()
{
	if (renderer_ != nullptr)
		SDL_DestroyRenderer(renderer_);

	if (window_ != nullptr)
		SDL_DestroyWindow(window_);
}

bool WindowHandler::initialize(const std::string& name,
                               const SDL_Rect dimensions,
                               SDL_Surface* icon)
{
	if (initialized_) {
		Engine::log << Priority::warn
		            << "WindowHandler was already initialized."
		            << std::endl;
		return false;
	}

	this->logVideoInfo();

	if (dimensions.w <= 0 || dimensions.h <= 0 ) {
		Engine::log << Priority::error << "Invalid window dimensions: "
		            << dimensions.w <<'x'<< dimensions.h << std::endl;
		return false;
	}

	//create window
	const SDL_Rect& dim = dimensions;
	window_ = SDL_CreateWindow(name.c_str(), (dim.x >= 0) ? dim.x : 0,
	                           (dim.y >= 0) ? dim.y : 0, dim.w,dim.h,
	                           windowFlags_);

	if (window_ == nullptr) {
		Engine::log << Priority::error << "Failed to create SDL window."
		            << std::endl << SDL_GetError() << std::endl;
		return false;
	}

	Engine::log << Priority::debug
	            << "Window ID: " << SDL_GetWindowID(window_) << std::endl;
	Engine::log << Priority::debug
	            << "Window size: " << dim.w << 'x' << dim.h << "px"
	            << std::endl;
	Engine::log << Priority::debug
	            << "Window flags: " << SDL_GetWindowFlags(window_)
	            << std::endl;

	renderer_ = SDL_CreateRenderer(window_, -1, rendererFlags_);

	if (renderer_ == nullptr) {
		Engine::log << Priority::error << "Failed to create SDL renderer."
		            << std::endl << SDL_GetError() << std::endl;
		return false;
	}


	if (showCursor_)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);

	if (icon != nullptr)
		SDL_SetWindowIcon(window_, icon);

	initialized_ = true;
	Engine::log << Priority::info << "WindowHandler initialized."
	            << std::endl;
	return true;
}

void WindowHandler::toggleFullscreen()
{
	if (fullscreen_) {
		SDL_SetWindowFullscreen(window_, windowFlags_);
		fullscreen_ = false;
		Engine::log << Priority::verbose << "Exiting fullscreen mode."
		            << std::endl;
	} else {
		SDL_SetWindowFullscreen(window_,
		                        windowFlags_|SDL_WINDOW_FULLSCREEN |SDL_WINDOW_INPUT_GRABBED);
		fullscreen_ = true;
		Engine::log << Priority::verbose << "Entering fullscreen mode."
		            << std::endl;
	}
}

void WindowHandler::showCursor(const bool value)
{
	showCursor_ = value;

	if (showCursor_)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

bool WindowHandler::hasToRedraw() const
{
	return redraw_;
}

void WindowHandler::setRedrawFlag(const bool value)
{
	redraw_ = value;
}

bool WindowHandler::isResizable() const
{
	return resizable_;
}

bool  WindowHandler::isInFullscreen() const
{
	return fullscreen_;
}

SDL_Renderer* WindowHandler::getRenderer() const
{
	return renderer_;
}

void WindowHandler::logVideoInfo() const
{
	std::string buff = "Availible video drivers: ";
	buff += SDL_GetVideoDriver(0);

	for(int i=1; i<SDL_GetNumVideoDrivers(); i++) {
		buff += ", ";
		buff += SDL_GetVideoDriver(i);
	}
	Engine::log << Priority::verbose << buff << std::endl;
	Engine::log << Priority::info << "Current video driver: "
	            << SDL_GetCurrentVideoDriver() << std::endl;

	SDL_RendererInfo info;
	SDL_GetRenderDriverInfo(0, &info);
	buff.clear();
	buff += "Availible render drivers: ";
	buff += info.name;
	for(int i=1; i<SDL_GetNumRenderDrivers(); i++) {
		SDL_GetRenderDriverInfo(i, &info);
		buff += ", ";
		buff += info.name;
	}

	Engine::log << Priority::verbose << buff << std::endl;
}


}
