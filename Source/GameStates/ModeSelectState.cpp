#include "ModeSelectState.h"
#include "../Game.h"
#include "../Graphics/TextureCache.h"
#include "../AudioCache.h"
#include "GameStateStack.h"
#include "GameplayState.h"
#include "../Utilities/Utilities.h"

#include <SDL_video.h>
#include <SDL_rect.h>

#include <gsl/util>

namespace ttt::gs
{
	ModeSelectState::ModeSelectState(Game &game)
		: game{game}
	{
		SDL_Point windowSize;
		SDL_GetWindowSize(game.getWindow(), &windowSize.x, &windowSize.y);
		
		for (int i{0}; i < 2; ++i)
		{
			gsl::at(buttons, i).setTexture(game.textureCache[0], SDL_Rect{0, 384 + 64 * i, 64, 64}, SDL_Rect{64, 384 + 64 * i, 64, 64}, SDL_Rect{128, 384 + 64 * i, 64, 64});
			gsl::at(buttons, i).setDestination({windowSize.x / 2.0f - 66.5f, 100.0f + 138.0f * i, 127.0f, 127.0f});
			gsl::at(buttons, i).setSound(game.audioCache.getChunk(0));
		}

		gsl::at(buttons, 2).setTexture(game.textureCache[0], SDL_Rect{0, 256, 64, 64}, SDL_Rect{64, 256, 64, 64}, SDL_Rect{128, 256, 64, 64});
		gsl::at(buttons, 2).setDestination({windowSize.x / 2.0f - 66.5f, 100.0f + 138.0f * 2, 127.0f, 127.0f});
		gsl::at(buttons, 2).setSound(game.audioCache.getChunk(0));
	}

	void ModeSelectState::update() noexcept
	{
		for (gfx::Button &button : buttons)
			button.update();

		if (buttons[0].wasReleased())
		{
			game.gameStateStack.clear();
			game.gameStateStack.emplace<GameplayState>(game, GameMode::vsBot);
		}
		else if (buttons[1].wasReleased())
		{
			game.gameStateStack.clear();
			game.gameStateStack.emplace<GameplayState>(game, GameMode::vsPlayer);
		}
		else if (buttons[2].wasReleased())
			game.gameStateStack.pop();
	}

	void ModeSelectState::render() const noexcept
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