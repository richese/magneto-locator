#ifndef _SDL2_ENGINE_WINDOW_H_
#define _SDL2_ENGINE_WINDOW_H_

#include <string>

#include <SDL2/SDL.h>


namespace engine {


class WindowHandler
{
	bool initialized_;
	bool redraw_;

	bool resizable_;
	bool showCursor_;
	bool fullscreen_;

	SDL_Window*   window_;
	SDL_Renderer* renderer_;
	Uint32 windowFlags_;
	Uint32 rendererFlags_;

public:
	WindowHandler();
	WindowHandler(const bool resizable, const bool cursor,
	              const bool fullscreen);
	virtual ~WindowHandler();

	bool initialize(const std::string& name, const SDL_Rect dimensions,
	                SDL_Surface* icon);

	void toggleFullscreen();
	void showCursor(const bool value);
	bool isResizable()    const;
	bool isInFullscreen() const;

	bool hasToRedraw() const;
	void setRedrawFlag(const bool value = true);

	SDL_Renderer* getRenderer() const;

protected:
	void logVideoInfo() const;
};


}


#endif // _SDL2_ENGINE_WINDOW_H_
