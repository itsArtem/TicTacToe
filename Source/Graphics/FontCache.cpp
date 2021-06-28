#include "FontCache.h"
#include "../Utilities/Exceptions.h"

#include <utility>

namespace ttt::gfx
{
	TTF_Font *FontCache::load(std::string_view path, int size, int index)
	{
		TTF_Font *const font{TTF_OpenFontIndex(path.data(),  size, index)};
		if (!font)
			throw util::IoError{SDL_GetError()};
		fonts.emplace_back(font, TTF_CloseFont);
		return font;
	}
}