#include "Game.h"
#include "Utilities/Exceptions.h"
#include "GameStates/MainMenuState.h"

#include <SDL_events.h>

#include <chrono>
#include <iostream>

namespace ttt
{
	void Game::run()
	{
		namespace ch = std::chrono;

		init();
		running = true;

		const float timePerFrame{1e9f / updatesPerSecond};
		auto lastItTime{ch::steady_clock::now()};
		float delta{0.0f};

#ifdef TTT_DEBUG
		int updates{0};
		int frames{0};  
		auto lastCounterTime{ch::steady_clock::now()};
#endif

		while (running)
		{
			const auto now{ch::steady_clock::now()};
			delta += (now - lastItTime).count() / timePerFrame;
			lastItTime = now;

			if (delta > 0.0f)
			{
				update();
				--delta;
#ifdef TTT_DEBUG
				++updates;
#endif
			}

			render();
#ifdef TTT_DEBUG
			++frames;

			if (now - lastCounterTime >= ch::seconds{1})
			{
				lastCounterTime = now;
				std::cout << frames << " FPS, " << updates << " UPS.\n";
				updates = frames = 0;
			}
#endif
		}
	}

	void Game::terminate() noexcept
	{
		running = false;
	}

	void Game::init()
	{
		window.reset(SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 560, 640, SDL_WINDOW_SHOWN));
		renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));

		if (!window || !renderer)
			throw util::ExternalError{SDL_GetError()};

		gameStateStack.emplace<gs::MainMenuState>(*this);
	}

	void Game::update()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				terminate();

		gameStateStack.update();
	}

	void Game::render() const noexcept
	{
		gameStateStack.render();

		SDL_RenderPresent(renderer.get());
		SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
		SDL_RenderClear(renderer.get());
	}
}