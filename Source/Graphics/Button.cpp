#include "Button.h"

#include <gsl/assert>
#include <gsl/util>

#include <cstdint>

namespace ttt::gfx
{
	Button::Button(SDL_Texture *texture, const SDL_Rect &normalSrc, const SDL_FRect &dst, Mix_Chunk *sound) noexcept
		: texture{texture},
		normalSrc{normalSrc},
		dst{dst},
		sound{sound}
	{
		Expects(normalSrc.x >= 0 && normalSrc.y >= 0 && normalSrc.w >= 0 && normalSrc.h >= 0);
		Expects(dst.w >= 0 && dst.h >= 0);
	}

	Button::Button(SDL_Texture *texture, const SDL_Rect &normalSrc, const std::optional<SDL_Rect> &hoverSrc, const std::optional<SDL_Rect> &pressedSrc, const SDL_FRect &dst, Mix_Chunk *sound) noexcept
		: texture{texture},
		normalSrc{normalSrc},
		hoverSrc{hoverSrc},
		pressedSrc{pressedSrc},
		dst{dst},
		sound{sound}
	{
		Expects(normalSrc.x >= 0 && normalSrc.y >= 0 && normalSrc.w >= 0 && normalSrc.h >= 0);
		Expects(!hoverSrc || hoverSrc->x >= 0 && hoverSrc->y >= 0 && hoverSrc->w >= 0 && hoverSrc->h >= 0);
		Expects(!pressedSrc || pressedSrc->x >= 0 && pressedSrc->y >= 0 && pressedSrc->w >= 0 && pressedSrc->h >= 0);
		
		Expects(dst.w >= 0 && dst.h >= 0);
	}

	void Button::setTexture(SDL_Texture *texture, const SDL_Rect &normalSrc) noexcept
	{
		Expects(normalSrc.x >= 0 && normalSrc.y >= 0 && normalSrc.w >= 0 && normalSrc.h >= 0);

		this->texture = texture;
		this->normalSrc = normalSrc;
	}

	void Button::setTexture(SDL_Texture *texture, const SDL_Rect &normalSrc, const std::optional<SDL_Rect> &hoverSrc, const std::optional<SDL_Rect> &pressedSrc) noexcept
	{
		Expects(normalSrc.x >= 0 && normalSrc.y >= 0 && normalSrc.w >= 0 && normalSrc.h >= 0);
		Expects(!hoverSrc || hoverSrc->x >= 0 && hoverSrc->y >= 0 && hoverSrc->w >= 0 && hoverSrc->h >= 0);
		Expects(!pressedSrc || pressedSrc->x >= 0 && pressedSrc->y >= 0 && pressedSrc->w >= 0 && pressedSrc->h >= 0);

		this->texture = texture;
		this->normalSrc = normalSrc;
		this->hoverSrc = hoverSrc;
		this->pressedSrc = pressedSrc;
	}

	void Button::setDestination(const SDL_FRect &dst) noexcept
	{
		Expects(dst.w >= 0 && dst.h >= 0);
		this->dst = dst;
	}
	
	void Button::update() noexcept
	{
		SDL_Point mouse;
		// SDL is a C library, and sadly C doesn't have templates, so you have to resort to horrible things such as these casts...
		SDL_Rect bounds{gsl::narrow_cast<int>(dst.x), gsl::narrow_cast<int>(dst.y), gsl::narrow_cast<int>(dst.w), gsl::narrow_cast<int>(dst.h)};
		const std::uint32_t button{SDL_GetMouseState(&mouse.x, &mouse.y)};
		
		if ((button & SDL_BUTTON_LMASK) != 0)
		{
			if (!click) click = mouse;
		}
		else
			click = std::nullopt;

		hoveredOver = SDL_PointInRect(&mouse, &bounds);
		released = pressed && !click && hoveredOver;
		pressed = click && SDL_PointInRect(&(*click), &bounds);

		if (released)
			Mix_PlayChannel(-1, sound, 0);
	}

	void Button::render(gsl::not_null<SDL_Renderer *> renderer) const noexcept
	{
		if (pressedSrc && isPressed())
			SDL_RenderCopyF(renderer, texture, &(*pressedSrc), &dst);
		else if (hoverSrc && isHoveredOver())
			SDL_RenderCopyF(renderer, texture, &(*hoverSrc), &dst);
		else
			SDL_RenderCopyF(renderer, texture, &normalSrc, &dst);
	}
}