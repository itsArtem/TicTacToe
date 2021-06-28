#ifndef TTT_GS_MODESELECTSTATE_H
#define TTT_GS_MODESELECTSTATE_H

#include "GameState.h"
#include "../Game.h"
#include "../Graphics/Text.h"
#include "../Graphics/Button.h"

#include <array>

namespace ttt::gs
{
	class ModeSelectState final : public GameState
	{
	public:
		ModeSelectState(Game &game);

		void update() noexcept override;
		void render() const noexcept override;

	private:
		Game &game;

		gfx::Text title{"Mode Select", game.fontCache[0], {255, 150, 50, 255}, true, game.getRenderer()};
		std::array<gfx::Button, 3> buttons;
	};
}

#endif