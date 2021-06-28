#ifndef TTT_GAME_H
#define TTT_GAME_H

#include "Graphics/TextureCache.h"
#include "Graphics/FontCache.h"
#include "AudioCache.h"
#include "GameStates/GameStateStack.h"

#include <SDL_video.h>
#include <SDL_render.h>

#include <memory>

namespace ttt
{
	// The main program class.
	class Game final
	{
	public:
		static constexpr int updatesPerSecond{60};

		gfx::TextureCache textureCache;
		gfx::FontCache fontCache;
		AudioCache audioCache;

		gs::GameStateStack gameStateStack;

		void run();
		void terminate() noexcept;

		[[nodiscard]] SDL_Window *getWindow() const noexcept;
		[[nodiscard]] SDL_Renderer *getRenderer() const noexcept;

	private:
		bool running{false};

		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window{nullptr, SDL_DestroyWindow};
		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer{nullptr, SDL_DestroyRenderer};

		void init();

		void update();
		void render() const noexcept;
	};

	inline SDL_Window *Game::getWindow() const noexcept { return window.get(); }
	inline SDL_Renderer *Game::getRenderer() const noexcept { return renderer.get(); }
}

#endif