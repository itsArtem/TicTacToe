#ifndef TTT_GS_PAUSEMENUSTATE_H
#define TTT_GS_PAUSEMENUSTATE_H

#include "GameState.h"
#include "../Game.h"
#include "../Graphics/Text.h"
#include "../Graphics/Button.h"

#include <array>

namespace ttt::gs
{
	class PauseMenuState final : public GameState
	{
	public:
		PauseMenuState(Game &game);

		void update() noexcept override;
		void render() const noexcept override;

	private:
		Game &game;

		gfx::Text title{"Pause Menu", game.fontCache[0], {255, 150, 50, 255}, true, game.getRenderer()};
		std::array<gfx::Button, 2> buttons;
		// Used to make sure that the game will not resume until [esc] has been released. This should probably be handled by some function, but whatever.
		bool pressedResume{false};
	};
}

#endif