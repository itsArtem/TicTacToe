#ifndef TTT_GS_ABOUTMENUSTATE_H
#define TTT_GS_ABOUTMENUSTATE_H

#include "GameState.h"
#include "../Game.h"
#include "../Graphics/Text.h"
#include "../Graphics/Button.h"

#include <array>

namespace ttt::gs
{
	class AboutMenuState final : public GameState
	{
	public:
		AboutMenuState(Game &game) noexcept;

		void update() noexcept override;
		void render() const noexcept override;

	private:
		Game &game;
		
		gfx::Text title{"About", game.fontCache[0], {255, 150, 50, 255}, true, game.getRenderer()};
		std::array<gfx::Text, 3> textLines{ // A lazy hack.
			gfx::Text{"Created by Artem Katerynych. Written from scratch in", game.fontCache[1], {255, 255, 255, 255}, true, game.getRenderer()},
			gfx::Text{"C++ using SDL2 for a coding challenge.", game.fontCache[1], {255, 255, 255, 255}, true, game.getRenderer()},
			gfx::Text{"-June 28th, 2021", game.fontCache[1], {255, 255, 255, 255}, true, game.getRenderer()}
		};
		
		gfx::Button back;
	};
}

#endif