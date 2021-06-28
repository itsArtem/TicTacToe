#include "AboutMenuState.h"
#include "../Graphics/TextureCache.h"
#include "../AudioCache.h"
#include "GameStateStack.h"

#include <SDL_video.h>
#include <SDL_rect.h>

#include <gsl/pointers>

namespace ttt::gs
{
	AboutMenuState::AboutMenuState(Game &game) noexcept
		: game{game}
	{
		SDL_Point windowSize;
		SDL_GetWindowSize(game.getWindow(), &windowSize.x, &windowSize.y);

		back.setTexture(game.textureCache[0], SDL_Rect{0, 256, 64, 64}, SDL_Rect{64, 256, 64, 64}, SDL_Rect{128, 256, 64, 64});
		back.setDestination({windowSize.x / 2 - 50.0f, windowSize.y - 137.0f, 100.0f, 100.0f});
		back.setSound(game.audioCache.getChunk(0));
	}

	void AboutMenuState::update() noexcept
	{
		back.update();
		if (back.wasReleased())
			game.gameStateStack.pop();
	}

	void AboutMenuState::render() const noexcept
	{
		const gsl::not_null<SDL_Renderer *> renderer{game.getRenderer()};

		SDL_SetRenderDrawColor(renderer, 150, 50, 50, 255);
		SDL_RenderClear(renderer);

		SDL_Point windowSize;
		SDL_GetWindowSize(game.getWindow(), &windowSize.x, &windowSize.y);
		
		title.render(renderer, {windowSize.x / 2.0f - title.getSize().x / 2.0f, 10});
		for (int i{0}; i < 3; ++i)
		{
			const gfx::Text &line{gsl::at(textLines, i)};
			line.render(renderer, {windowSize.x / 2.0f - line.getSize().x / 2.0f, 100.0f + line.getSize().y * i});
		}

		back.render(renderer);
	}
}
