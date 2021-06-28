#include "PauseMenuState.h"
#include "../Game.h"
#include "../Graphics/TextureCache.h"
#include "GameStateStack.h"
#include "MainMenuState.h"
#include "../Utilities/Utilities.h"

#include <SDL_video.h>
#include <SDL_rect.h>

#include <gsl/util>

namespace ttt::gs
{
	PauseMenuState::PauseMenuState(Game &game)
		: game{game}
	{
		SDL_Point windowSize;
		SDL_GetWindowSize(game.getWindow(), &windowSize.x, &windowSize.y);
		
		gsl::at(buttons, 0).setTexture(game.textureCache[0], SDL_Rect{0, 0, 64, 64}, SDL_Rect{64, 0, 64, 64}, SDL_Rect{128, 0, 64, 64});
		gsl::at(buttons, 1).setTexture(game.textureCache[0], SDL_Rect{0, 128, 64, 64}, SDL_Rect{64, 128, 64, 64}, SDL_Rect{128, 128, 64, 64});

		for (int i{0}; i < 2; ++i)
		{
			gsl::at(buttons, i).setDestination({windowSize.x / 2.0f - 66.5f, 100.0f + 138.0f * i, 127.0f, 127.0f});
			gsl::at(buttons, i).setSound(game.audioCache.getChunk(0));
		}
	}

	void PauseMenuState::update() noexcept
	{
		for (gfx::Button &button : buttons)
			button.update();

		if (buttons[0].wasReleased())
			game.gameStateStack.pop();
		else if (buttons[1].wasReleased())
		{
			game.gameStateStack.clear();
			game.gameStateStack.emplace<MainMenuState>(game);
		}
	}

	void PauseMenuState::render() const noexcept
	{
		const gsl::not_null<SDL_Renderer *> renderer{game.getRenderer()};

		SDL_SetRenderDrawColor(renderer, 150, 50, 50, 255);
		SDL_RenderClear(renderer);

		SDL_Point windowSize;
		SDL_GetWindowSize(game.getWindow(), &windowSize.x, &windowSize.y);
		title.render(renderer, {windowSize.x / 2.0f - title.getSize().x / 2.0f, 10});

		for (const gfx::Button &button : buttons)
			button.render(renderer);
	}
}