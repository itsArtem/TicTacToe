#ifndef TTT_UTIL_FONTCACHE_H
#define TTT_UTIL_FONTCACHE_H

#include <SDL_ttf.h>

#include <gsl/pointers>
#include <gsl/util>

#include <vector>
#include <memory>
#include <string_view>

namespace ttt::gfx
{
	// A class for caching fonts for reuse.
	class FontCache final
	{
	public:
		using SizeType = gsl::index;

		TTF_Font  *load(std::string_view path, int size, int index = 0);
		void unload(SizeType index) noexcept;
		void erase(SizeType index) noexcept;
		void clear() noexcept;

		[[nodiscard]] TTF_Font *at(SizeType index) const noexcept;
		[[nodiscard]] TTF_Font *operator [](SizeType index) const noexcept;
		
		[[nodiscard]] SizeType getSize() const noexcept;
		[[nodiscard]] SizeType getMaxSize() const noexcept;

	private:
		std::vector<std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>> fonts;
	};

	inline void FontCache::unload(SizeType index) noexcept { gsl::at(fonts, index).reset(); }
	inline void FontCache::erase(SizeType index) noexcept { fonts.erase(fonts.cbegin() + index); }
	inline void FontCache::clear() noexcept { fonts.clear(); }

	inline TTF_Font *FontCache::at(SizeType index) const noexcept { return gsl::at(fonts, index).get(); }
	inline TTF_Font *FontCache::operator [](SizeType index) const noexcept { return gsl::at(fonts, index).get(); }

	inline FontCache::SizeType FontCache::getSize() const noexcept { return fonts.size(); }
	inline FontCache::SizeType FontCache::getMaxSize() const noexcept { return fonts.max_size(); }
}

#endif