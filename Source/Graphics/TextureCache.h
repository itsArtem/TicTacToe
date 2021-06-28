#ifndef TTT_UTIL_TEXTURECACHE_H
#define TTT_UTIL_TEXTURECACHE_H

#include <SDL_render.h>

#include <gsl/pointers>
#include <gsl/util>

#include <vector>
#include <memory>
#include <string_view>

namespace ttt::gfx
{
	// A class for caching textures for reuse.
	class TextureCache final
	{
	public:
		using SizeType = gsl::index;

		SDL_Texture *load(std::string_view path, gsl::not_null<SDL_Renderer *> renderer);
		void unload(SizeType index) noexcept;
		void erase(SizeType index) noexcept;
		void clear() noexcept;

		[[nodiscard]] SDL_Texture *at(SizeType index) const noexcept;
		[[nodiscard]] SDL_Texture *operator [](SizeType index) const noexcept;
		
		[[nodiscard]] SizeType getSize() const noexcept;
		[[nodiscard]] SizeType getMaxSize() const noexcept;

	private:
		std::vector<std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>> textures;
	};

	inline void TextureCache::unload(SizeType index) noexcept { gsl::at(textures, index).reset(); }
	inline void TextureCache::erase(SizeType index) noexcept { textures.erase(textures.cbegin() + index); }
	inline void TextureCache::clear() noexcept { textures.clear(); }

	inline SDL_Texture *TextureCache::at(SizeType index) const noexcept { return gsl::at(textures, index).get(); }
	inline SDL_Texture *TextureCache::operator [](SizeType index) const noexcept { return gsl::at(textures, index).get(); }

	inline TextureCache::SizeType TextureCache::getSize() const noexcept { return textures.size(); }
	inline TextureCache::SizeType TextureCache::getMaxSize() const noexcept { return textures.max_size(); }
}

#endif