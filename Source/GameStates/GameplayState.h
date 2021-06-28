#ifndef GS_GAMEPLAYSTATE_H
#define GS_GAMEPLAYSTATE_H

#include "GameState.h"
#include "../Game.h"
#include "../Board.h"
#include "../Graphics/Text.h"
#include "../Graphics/Button.h"
#include "../Graphics/TextureCache.h"
#include "../AudioCache.h"

#include <SDL_pixels.h>

#include <optional>
#include <random>

namespace ttt::gs
{
	enum class GameMode
	{
		vsPlayer, vsBot
	};

	enum class Turn
	{
		player1, player2
	};

	struct BotData final
	{
		std::default_random_engine randEngine;
		SDL_Point lastPlayerMove{0, 0};

		static constexpr float delayTime{500.0f};
		float delayTimer{delayTime};

		BotData() = default;
		BotData(std::random_device &randDevice) noexcept;
	};

	inline BotData::BotData(std::random_device &randDevice) noexcept
		: randEngine{randDevice()} { }

	class GameplayState final : public GameState
	{
	public:
		GameplayState(Game &game, GameMode mode);

		void update() override;
		void render() const noexcept override;

	private:
		enum class LineType
		{
			horizontal, vertical
		};

		enum class CrossType
		{
			leftToRight, rightToLeft
		};

		Game &game;

		Board board{{0.0f, 0.0f}, {160.0f , 160.0f}, game};
		GameMode mode;

		std::random_device randDevice;
		Turn turn{static_cast<Turn>(randDevice() % 2)};
		std::optional<BotData> botData{mode == GameMode::vsBot ? std::optional<BotData>{BotData{randDevice}} : std::nullopt};

		static constexpr SDL_Colour player1Colour{61, 81, 255, 255};
		static constexpr SDL_Colour player2Colour{255, 146, 40, 255};

		int score1{0};
		int score2{0};
		gfx::Text scoreText1;
		gfx::Text scoreText2;
		const gfx::Text score1Sub{"P1", game.fontCache[3], player1Colour, true, game.getRenderer()};
		const gfx::Text score2Sub{"P2", game.fontCache[3], player2Colour, true, game.getRenderer()};

		bool gameOverDone{false};
		static constexpr float gameOverTime{3000.0f};
		float gameOverTimer{gameOverTime};
		gfx::Text stateText;

		const SDL_Rect panelSrc{0, 320, 192, 64};
		SDL_FRect panelDst{};

		gfx::Button pause{game.textureCache[0], SDL_Rect{0, 192, 64, 64}, SDL_Rect{64, 192, 64, 64}, SDL_Rect{128, 192, 64, 64}, SDL_FRect{2.0f, 2.0f, 38.0f, 38.0f}, game.audioCache.getChunk(0)};

		void getInput() noexcept;

		bool counter(const SDL_Point &cell) noexcept;
		[[nodiscard]] SDL_Point getRandomCell() noexcept;
		
		[[nodiscard]] bool lineCheck() const noexcept;
		[[nodiscard]] bool lineCheck(LineType type) const noexcept;

		[[nodiscard]] bool crossCheck() const noexcept;
		[[nodiscard]] bool crossCheck(CrossType type) const noexcept;

		[[nodiscard]] bool fullCheck() const noexcept;
	};

	inline SDL_Point GameplayState::getRandomCell() noexcept
	{
		return SDL_Point{randDevice() % 3, randDevice() % 3};
	}

	inline bool GameplayState::lineCheck() const noexcept
	{
		return lineCheck(LineType::horizontal) || lineCheck(LineType::vertical);
	}

	inline bool GameplayState::crossCheck() const noexcept
	{
		return crossCheck(CrossType::leftToRight) || crossCheck(CrossType::rightToLeft);
	}
}

#endif