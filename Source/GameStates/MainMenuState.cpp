#include "MainMenuState.h"
#include "../Game.h"
#include "../Graphics/TextureCache.h"
#include "../Graphics/FontCache.h"
#include "../AudioCache.h"
#include "GameStateStack.h"
#include "AboutMenuState.h"
#include "ModeSelectState.h"
#include "../Utilities/Utilities.h"
#include "../Utilities/Exceptions.h"

#include <SDL_video.h>
#include <SDL_rect.h>
#include <SDL_mixer.h>

#include <gsl/util>

#include <fstream>

namespace ttt::gs
{
	MainMenuState::MainMenuState(Game &game)
		: game{game}
	{
		game.textureCache.load("Resources/UiSheet.png", game.getRenderer());
		game.fontCache.load("Resources/BebasNeue-Regular.ttf", 65);
		game.fontCache.load("Resources/Renner.ttf", 24);
		game.fontCache.load("Resources/Renner.ttf", 38);
		game.fontCache.load("Resources/Renner.ttf", 24);

		game.audioCache.loadChunk("Resources/Click.wav");
		Mix_PlayMusic(game.audioCache.loadMusic("Resources/Background.mid"), -1);
		
		title.setText("Tic-Tac-Toe", game.fontCache[0], {255, 150, 50, 255}, true, game.getRenderer());
		
		SDL_Point windowSize;
		SDL_GetWindowSize(game.getWindow(), &windowSize.x, &windowSize.y);

		for (int i{0}; i < 3; ++i)
		{
			gsl::at(buttons, i).setTexture(game.textureCache[0], SDL_Rect{0, 64 * i, 64, 64}, SDL_Rect{64, 64 * i, 64, 64}, SDL_Rect{128, 64 * i, 64, 64});
			gsl::at(buttons, i).setDestination({windowSize.x / 2.0f - 66.5f, 100.0f + 138.0f * i, 127.0f, 127.0f});
			gsl::at(buttons, i).setSound(game.audioCache.getChunk(0));
		}

		gsl::at(buttons, 3).setTexture(game.textureCache[0], SDL_Rect{0, 512, 64, 64}, SDL_Rect{64, 512, 64, 64}, SDL_Rect{128, 512, 64, 64});
		gsl::at(buttons, 3).setDestination({4.0f, windowSize.y - 68.0f, 64.0f, 64.0f});
		gsl::at(buttons, 3).setSound(game.audioCache.getChunk(0));
	}
	
	void MainMenuState::update()
	{
		for (gfx::Button &button : buttons)
			button.update();
		
		if (buttons[0].wasReleased())
			game.gameStateStack.emplace<ModeSelectState>(game);
		else if (buttons[1].wasReleased())
			game.gameStateStack.emplace<AboutMenuState>(game);
		else if (buttons[2].wasReleased())
			game.terminate();
		else if (buttons[3].wasReleased())
		{
			std::ofstream stream{"Resources/Score.sav", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary};
			if (!stream)
				throw util::IoError{"Could not open save file for writing."};

			constexpr int value{0};
			stream.write(reinterpret_cast<const char *>(&value), sizeof(value));
			stream.write(reinterpret_cast<const char *>(&value), sizeof(value));
		}
	}

	void MainMenuState::render() const noexcept
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