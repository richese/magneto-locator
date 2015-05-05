#ifndef _SDL2_ENGINE_MODULE_FONT_H_
#define _SDL2_ENGINE_MODULE_FONT_H_

#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


namespace engine {


enum _FontStyles {
	STYLE_NORMAL = TTF_STYLE_NORMAL,
	STYLE_BOLD = TTF_STYLE_BOLD,
	STYLE_ITALIC = TTF_STYLE_ITALIC,
	STYLE_UNDERLINE = TTF_STYLE_UNDERLINE,
	STYLE_STRIKETHROUGH = TTF_STYLE_STRIKETHROUGH
};


typedef std::unordered_map<Uint16, TTF_Font*> FontMap_t;


class Font
{
	const char *fontPath_;
	FontMap_t font_;
	SDL_Color renderColor_;
	Uint8 renderStyle_;

public:
	Font(const char *fontPath, const Uint32 initialSizeCount = 5);
	~Font();


	void setRenderColor(const SDL_Color color);
	SDL_Color getRenderColor() const;
	void setRenderStyle(const Uint8 style);
	Uint8 getRenderStyle() const;


	SDL_Texture * render(const char *text, const Uint16 ptsize);
	SDL_Texture * render(const char *text, const Uint16 ptsize,
	                     const Uint8 style);
	SDL_Texture * render(const char *text, const Uint16 ptsize,
	                     const SDL_Color &color);
	SDL_Texture * render(const char *text, const Uint16 ptsize,
	                     const Uint8 style, const SDL_Color& color);

	SDL_Surface * renderSurface(const char *text, const Uint16 ptsize,
	                            const SDL_Color &color, const Uint8 style);

protected:
	void loadSize(const Uint16 ptsize);
};


} // namespace engine


#endif // _SDL2_ENGINE_MODULE_FONT_H_
