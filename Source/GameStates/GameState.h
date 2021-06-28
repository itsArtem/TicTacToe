#ifndef TTT_GS_GAMESTATES_H
#define TTT_GS_GAMESTATES_H

namespace ttt::gs
{
	class GameState
	{
	public:
		virtual ~GameState() = default;

		virtual void update() = 0;
		virtual void render() const noexcept = 0;

	protected:
		GameState() = default;
		GameState(const GameState &gameStates) = default;
		GameState(GameState &&gameStates) = default;

		GameState &operator =(const GameState &gameStates) = default;
		GameState &operator =(GameState && gameStates) = default;
	};
}

#endif