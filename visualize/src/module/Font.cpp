#include "Font.hpp"

#include <iostream>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "core/Engine.hpp"


namespace engine {


Font::Font(const char *fontPath, const Uint32 initialSizeCount) :
	fontPath_(fontPath),
	font_(initialSizeCount),
	renderColor_( {0, 0, 0, 255} ),  //black color
	renderStyle_(STYLE_NORMAL)
{
	if (fontPath_ == nullptr) {
		Engine::log << Priority::error << "Font path is invalid."
		            << std::endl;
		Engine::exit(1);
	}

	if(TTF_WasInit() == 0) {
		Engine::log << Priority::error
		            << "SDL_ttf was not initialized yet." << std::endl;
	}

}

Font::~Font()
{
	for (auto it=font_.begin(); it != font_.end(); ++it) {
		if (it->second != nullptr) {
			TTF_CloseFont(it->second);
			it->second = nullptr;
		}
	}
}

void Font::setRenderColor(const SDL_Color color)
{
	renderColor_ = color;
}

SDL_Color Font::getRenderColor() const
{
	return renderColor_;
}

void Font::setRenderStyle(const Uint8 style)
{
	renderStyle_ = style;
}

Uint8 Font::getRenderStyle() const
{
	return renderStyle_;
}

SDL_Texture * Font::render(const char *text, const Uint16 ptsize)
{
	SDL_Texture *result = nullptr;

	if (!Engine::wasCreated()) {
		Engine::log << Priority::error
		            << "Cannot render texture without engine renderer."
		            << std::endl;
		return nullptr;
	}

	SDL_Surface *surfaceRender = renderSurface(text, ptsize, renderColor_,
	                                           renderStyle_);

	result = SDL_CreateTextureFromSurface(Engine::get().getWindow().getRenderer(),
                                              surfaceRender);

	SDL_FreeSurface(surfaceRender);

	return result;
}

SDL_Texture * Font::render(const char *text, const Uint16 ptsize,
                          const Uint8 style)
{
	SDL_Texture *result = nullptr;

	if (!Engine::wasCreated()) {
		Engine::log << Priority::error
		            << "Cannot render texture without engine renderer."
		            << std::endl;
		return nullptr;
	}

	SDL_Surface *surfaceRender = renderSurface(text, ptsize, renderColor_,
	                                           style);

	result = SDL_CreateTextureFromSurface(Engine::get().getWindow().getRenderer(),
	                                      surfaceRender);

	SDL_FreeSurface(surfaceRender);

	return result;
}

SDL_Texture * Font::render(const char *text, const Uint16 ptsize,
                          const SDL_Colour &color)
{
	SDL_Texture *result = nullptr;

	if (!Engine::wasCreated()) {
		Engine::log << Priority::error
		            << "Cannot render texture without engine renderer."
		            << std::endl;
		return nullptr;
	}

	SDL_Surface *surfaceRender = renderSurface(text, ptsize, color,
	                                           renderStyle_);

	result = SDL_CreateTextureFromSurface(Engine::get().getWindow().getRenderer(),
	                                      surfaceRender);

	SDL_FreeSurface(surfaceRender);

	return result;
}

SDL_Texture * Font::render(const char *text, const Uint16 ptsize,
                          const Uint8 style, const SDL_Colour &color)
{
	SDL_Texture *result = nullptr;

	if (!Engine::wasCreated()) {
		Engine::log << Priority::error
		            << "Cannot render texture without engine renderer."
		            << std::endl;
		return nullptr;
	}

	SDL_Surface *surfaceRender = renderSurface(text, ptsize, color, style);

	result = SDL_CreateTextureFromSurface(Engine::get().getWindow().getRenderer(),
	                                      surfaceRender);

	SDL_FreeSurface(surfaceRender);

	return result;
}

SDL_Surface * Font::renderSurface(const char *text, const Uint16 ptsize,
                                 const SDL_Colour &color, const Uint8 style)
{
	SDL_Surface *result = nullptr;

	if (font_.count(ptsize) == 0)
		loadSize(ptsize);

	TTF_SetFontStyle(font_[ptsize], style);

	result = TTF_RenderText_Blended(font_[ptsize], text, color);

	if (result == nullptr) {
		Engine::log << Priority::error << TTF_GetError() << std::endl;
		return nullptr;
	}

	return result;
}

void Font::loadSize(const Uint16 ptsize)
{
	//chech if font was already loaded
	if (font_.count(ptsize) == 1 )
		return;

	TTF_Font *tmp = TTF_OpenFont(fontPath_, ptsize);

	if (tmp == nullptr) {
		Engine::log << Priority::error << TTF_GetError() << std::endl;
		Engine::exit(1);
	}

	font_[ptsize] = tmp;
}



} // namespace engine
