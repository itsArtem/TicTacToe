#ifndef TTT_GS_MAINMENUSTATE_H
#define TTT_GS_MAINMENUSTATE_H

#include "GameState.h"
#include "../Graphics/Text.h"
#include "../Graphics/Button.h"

#include <array>

namespace ttt
{
	class Game;
}

namespace ttt::gs
{
	class MainMenuState final : public GameState
	{
	public:
		MainMenuState(Game &game);

		void update() override;
		void render() const noexcept override;

	private:
		Game &game;

		gfx::Text title;
		std::array<gfx::Button, 4> buttons;
	};
}

#endif