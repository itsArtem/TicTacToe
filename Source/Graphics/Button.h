#ifndef TTT_GFX_BUTTON_H
#define TTT_GFX_BUTTON_H

#include <SDL_render.h>
#include <SDL_mouse.h>
#include <SDL_mixer.h>

#include <gsl/pointers>

#include <optional>

namespace ttt::gfx
{
	// A basic clickable image button.
	class Button final
	{
	public:
		Button() = default;
		Button(SDL_Texture *texture, const SDL_Rect &normalSrc, const SDL_FRect &dst, Mix_Chunk *sound) noexcept;
		Button(SDL_Texture *texture, const SDL_Rect &normalSrc, const std::optional<SDL_Rect> &hoverSrc, const std::optional<SDL_Rect> &pressedSrc, const SDL_FRect &dst, Mix_Chunk *sound) noexcept;

		void setTexture(SDL_Texture *texture, const SDL_Rect &normalSrc) noexcept;
		void setTexture(SDL_Texture *texture, const SDL_Rect &normalSrc, const std::optional<SDL_Rect> &hoverSrc, const std::optional<SDL_Rect> &pressedSrc) noexcept;
		void setDestination(const SDL_FRect &dst) noexcept;
		void setSound(Mix_Chunk *sound) noexcept;

		void update() noexcept;
		void render(gsl::not_null<SDL_Renderer *> renderer) const noexcept;

		[[nodiscard]] bool isHoveredOver() const noexcept;
		[[nodiscard]] bool isPressed() const noexcept;
		[[nodiscard]] bool wasReleased() const noexcept;

	private:
		SDL_Texture *texture{nullptr};
		
		SDL_Rect normalSrc{0, 0, 0, 0};
		std::optional<SDL_Rect> hoverSrc;
		std::optional<SDL_Rect> pressedSrc;
		
		SDL_FRect dst{0.0f, 0.0f, 0.0f, 0.0f};
		Mix_Chunk *sound{nullptr};

		bool hoveredOver{false};
		bool pressed{false};
		bool released{false};
		std::optional<SDL_Point> click;
	};

	inline void Button::setSound(Mix_Chunk *sound) noexcept { this->sound = sound; }

	inline bool Button::isHoveredOver() const noexcept { return hoveredOver; }
	inline bool Button::isPressed() const noexcept { return pressed; }
	inline bool Button::wasReleased() const noexcept { return released; }
}

#endif