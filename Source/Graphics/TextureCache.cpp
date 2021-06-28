#include "TextureCache.h"
#include "../Utilities/Exceptions.h"

#include <SDL_image.h>

#include <utility>

namespace ttt::gfx
{
	SDL_Texture *TextureCache::load(std::string_view path, gsl::not_null<SDL_Renderer *> renderer)
	{
		SDL_Texture *const texture{IMG_LoadTexture(renderer, path.data())};
		if (!texture)
			throw util::IoError{SDL_GetError()};
		textures.emplace_back(texture, SDL_DestroyTexture);
		return texture;
	}
}