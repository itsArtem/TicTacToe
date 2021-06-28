#ifndef TTT_GFX_TEXT_H
#define TTT_GFX_TEXT_H

#include <SDL_render.h>
#include <SDL_ttf.h>

#include <gsl/pointers>
#include <gsl/util>

#include <memory>
#include <string_view>
#include <string>
#include <stdexcept>
#include <cstdint>

namespace ttt::gfx
{
	class TextError final : public std::runtime_error
	{
	public:
		TextError(const std::string &msg) noexcept;
	};

	inline TextError::TextError(const std::string &msg) noexcept
		: runtime_error{msg}
	{
	}

	// A very simple class for rendering text.
	class Text final
	{
	public:
		Text() = default;
		Text(std::string_view str, gsl::not_null<TTF_Font *> font, const SDL_Colour &colour, bool shadow, gsl::not_null<SDL_Renderer *> renderer);

		void setText(std::string_view str, gsl::not_null<TTF_Font *> font, const SDL_Colour &colour, bool shadow, gsl::not_null<SDL_Renderer *> renderer);
		void clear() noexcept;

		void render(gsl::not_null<SDL_Renderer *> renderer, const SDL_FPoint &pos, double angle = 0.0, const SDL_FPoint *centre = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const noexcept;

		[[nodiscard]] SDL_Point getSize() const noexcept { return size; }

	private:
		using TexturePointer = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

		TexturePointer primary{nullptr, SDL_DestroyTexture};
		TexturePointer secondary{nullptr, SDL_DestroyTexture};
		SDL_Point size{0, 0};

		[[nodiscard]] TexturePointer createTexture(std::string_view str, gsl::not_null<TTF_Font *> font, const SDL_Colour &colour, gsl::not_null<SDL_Renderer *> renderer);
		[[nodiscard]] SDL_Colour createShadowColour(const SDL_Colour &colour) const noexcept;
	};

	inline SDL_Colour Text::createShadowColour(const SDL_Colour &colour) const noexcept
	{
		return {
			gsl::narrow_cast<std::uint8_t>(colour.r % 5),
			gsl::narrow_cast<std::uint8_t>(colour.g % 5),
			gsl::narrow_cast<std::uint8_t>(colour.b % 5),
			colour.a
		};
	}
}

#endif