#include "TextureMap.hpp"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "core/Engine.hpp"


namespace engine {


TextureMap::TextureMap(const Uint32 initialSize) :
	textures_(initialSize)
{

}

TextureMap::~TextureMap()
{
	for (auto it = textures_.begin(); it != textures_.end(); ++it) {
		if (it->second != nullptr) {
			SDL_DestroyTexture(it->second);
			it->second = nullptr;
		}
	}
}

void TextureMap::loadImage(const Uint32 id, const char* filename)
                          throw(std::invalid_argument, std::runtime_error)
{
	if (filename == nullptr)
		throw std::invalid_argument("Invalid image filename.");

	SDL_Surface *image = IMG_Load(filename);
	if (image == nullptr)
		throw std::runtime_error( IMG_GetError() );

	SDL_Texture *texture = SDL_CreateTextureFromSurface(Engine::get().getWindow().getRenderer(),
	                                                    image);
	if (texture == nullptr)
		throw std::runtime_error( SDL_GetError() );

	//check if texture already exist and free it
	if (textures_.count(id) == 1) {
		if (textures_[id] != nullptr)
			SDL_DestroyTexture(textures_[id]);
	}
	textures_[id] = texture;

	SDL_FreeSurface(image);
}


void TextureMap::addTexture(const Uint32 id, SDL_Texture* texture)
                           throw(std::invalid_argument)
{
	if (texture == nullptr)
		throw std::invalid_argument("Invalid pointer to texture.");

	//check if texture already exist and free it
	if (textures_.count(id) == 1) {
		if (textures_[id] != nullptr)
			SDL_DestroyTexture(textures_[id]);
	}

	textures_[id] = texture;
}


void TextureMap::addSurface(const Uint32 id, SDL_Surface* surface,
                            const bool freeSurf)
                           throw(std::invalid_argument, std::runtime_error)
{
	if (surface == nullptr)
		throw std::invalid_argument("Invalid pointer to surface.");

	SDL_Texture *texture = SDL_CreateTextureFromSurface(Engine::get().getWindow().getRenderer(),
														surface);
	if (texture == nullptr)
		throw std::runtime_error(SDL_GetError());


	//check if texture already exist and free it
	if (textures_.count(id) == 1) {
		if (textures_[id] != nullptr)
			SDL_DestroyTexture(textures_[id]);
	}
	textures_[id] = texture;

	if (freeSurf == true) {
		SDL_FreeSurface(surface);
		surface = nullptr;
	}
}


SDL_Texture *TextureMap::getTexture(const Uint32 id) const
{
	if (textures_.count(id) == 1) {
		return textures_.at(id);
	}

	Engine::log << Priority::error << "Texture with ID: " << id
	            << "does not exist." << std::endl;
	return nullptr;
}


Uint32 TextureMap::getSize() const
{
	return textures_.size();
}


} // namespace engine
