#include "Text.h"
#include "../Utilities/Exceptions.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 26812) // Disables warning about use of unscoped enums from SDL.

#endif

namespace ttt::gfx
{
	Text::Text(std::string_view str, gsl::not_null<TTF_Font *> font, const SDL_Colour &colour, bool shadow, gsl::not_null<SDL_Renderer *> renderer)
		: primary{createTexture(str, font, colour, renderer)},
		secondary{shadow ? createTexture(str, font, createShadowColour(colour), renderer) : TexturePointer{nullptr, SDL_DestroyTexture}}
	{
		if (TTF_SizeText(font, str.data(), &size.x, &size.y) == -1)
			throw TextError{SDL_GetError()};
	}

	void Text::setText(std::string_view str, gsl::not_null<TTF_Font *> font, const SDL_Colour &colour, bool shadow, gsl::not_null<SDL_Renderer *> renderer)
	{
		primary = createTexture(str, font, colour, renderer);
		if (shadow)
			secondary = createTexture(str, font, createShadowColour(colour), renderer);

		if (TTF_SizeText(font, str.data(), &size.x, &size.y) == -1)
			throw TextError{SDL_GetError()};
	}

	void Text::clear() noexcept
	{
		primary.reset();
		secondary.reset();
		size.x = size.y = 0;
	}

	void Text::render(gsl::not_null<SDL_Renderer *> renderer, const SDL_FPoint &pos, double angle, const SDL_FPoint *centre, SDL_RendererFlip flip) const noexcept
	{
		if (secondary)
		{
			const SDL_FRect dst2{pos.x + 2, pos.y + 2, gsl::narrow_cast<float>(size.x), gsl::narrow_cast<float>(size.y)};
			SDL_RenderCopyExF(renderer, secondary.get(), nullptr, &dst2, angle, centre, flip);
		}

		if (primary)
		{
			const SDL_FRect dst1{pos.x, pos.y, gsl::narrow_cast<float>(size.x), gsl::narrow_cast<float>(size.y)};
			SDL_RenderCopyExF(renderer, primary.get(), nullptr, &dst1, angle, centre, flip);
		}
	}

	Text::TexturePointer Text::createTexture(std::string_view str, gsl::not_null<TTF_Font *> font, const SDL_Colour &colour, gsl::not_null<SDL_Renderer *> renderer)
	{
		std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)> surface{TTF_RenderText_Blended(font, str.data(), colour), SDL_FreeSurface};
		if (!surface)
			throw TextError{SDL_GetError()};

		TexturePointer texture{SDL_CreateTextureFromSurface(renderer, surface.get()), SDL_DestroyTexture};
		if (!texture)
			throw util::ExternalError{SDL_GetError()};

		return texture;
	}
}

#ifdef _MSC_VER
#pragma warning(pop)

#endif