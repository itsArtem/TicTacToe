#ifndef TTT_GS_GAMESTATESTACK_H
#define TTT_GS_GAMESTATESTACK_H

#include "GameState.h"
#include "../Utilities/Utilities.h"

#include <gsl/util>
#include <gsl/assert>

#include <deque>
#include <memory>
#include <optional>

namespace ttt::gs
{
	class GameStateStack final
	{
	public:
		using SizeType = gsl::index;

		template<typename T, typename... Arg>
		void emplace(Arg &&...args) noexcept(std::is_nothrow_constructible_v<T, Arg...>);
		void pop() noexcept;
		void clear() noexcept;

		void update();
		void render() const noexcept;

		void setBackgroundRender(const std::optional<SizeType> &index) noexcept;
		void setBackgroundUpdate(const std::optional<SizeType> &index) noexcept;

		[[nodiscard]] SizeType getSize() const noexcept;
		[[nodiscard]] SizeType getMaxSize() const noexcept;

	private:
		std::deque<std::unique_ptr<GameState>> gameStates;
		std::optional<SizeType> bgUpdateIndex;
		std::optional<SizeType> bgRenderIndex;
		
		SizeType popIndex{-1};
		SizeType queuedForPop{0};
	};

	template<typename T, typename... Arg>
	void GameStateStack::emplace(Arg &&...args) noexcept(std::is_nothrow_constructible_v<T, Arg...>)
	{
		gameStates.emplace_back(std::make_unique<T>(std::forward<Arg>(args)...));
	}

	inline void GameStateStack::setBackgroundUpdate(const std::optional<SizeType> &index) noexcept
	{
		Expects(!index || *index < util::getSize(gameStates));
		bgUpdateIndex = index;
	}

	inline void GameStateStack::setBackgroundRender(const std::optional<SizeType> &index) noexcept
	{
		Expects(!index || *index < util::getSize(gameStates));
		bgRenderIndex = index;
	}

	inline GameStateStack::SizeType GameStateStack::getSize() const noexcept { return gameStates.size(); }
	inline GameStateStack::SizeType GameStateStack::getMaxSize() const noexcept { return gameStates.max_size(); }
}

#endif