#include "GameStateStack.h"

namespace ttt::gs
{
	void GameStateStack::pop() noexcept 
	{ 
		Expects(util::getSize(gameStates) > 0);

		popIndex = util::getSize(gameStates) - 1; 
		++queuedForPop;
	}

	void GameStateStack::clear() noexcept
	{
		popIndex = util::getSize(gameStates) - 1;
		queuedForPop = util::getSize(gameStates);
	}

	void GameStateStack::update()
	{
		// Removing states this way allows them to continue functioning until the next update even after poping themselves.
		while (queuedForPop > 0)
		{
			gameStates.erase(gameStates.cbegin() + popIndex--);
			--queuedForPop;
		}
		popIndex = 0;

		gameStates.back()->update();
		if (bgUpdateIndex)
			gsl::at(gameStates, *bgUpdateIndex)->update();
	}

	void GameStateStack::render() const noexcept
	{
		gameStates.back()->render();
		if (bgUpdateIndex)
			gsl::at(gameStates, *bgRenderIndex)->render();
	}
}