#ifndef _SDL2_ENGINE_MODULE_TEXTURE_MAP_H_
#define _SDL2_ENGINE_MODULE_TEXTURE_MAP_H_

#include <stdexcept>
#include <unordered_map>

#include <SDL2/SDL.h>


namespace engine {


typedef std::unordered_map<Uint32, SDL_Texture*> TextureMap_t;


class TextureMap
{
	TextureMap_t textures_;

public:
	TextureMap(const Uint32 initialSize = 10);
	virtual ~TextureMap();


	void loadImage(const Uint32 id, const char* filename)
	              throw(std::invalid_argument, std::runtime_error);

	void addTexture(const Uint32 id, SDL_Texture* texture)
	               throw(std::invalid_argument);

	void addSurface(const Uint32 id, SDL_Surface* surface,
	                const bool freeSurf = true)
	               throw(std::invalid_argument, std::runtime_error);


	SDL_Texture * getTexture(const Uint32 id) const;
	Uint32 getSize() const;
};


} // namespace engine



#endif // _SDL2_ENGINE_MODULE_TEXTURE_MAP_H_
