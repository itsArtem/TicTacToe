#include "GameplayState.h"
#include "../Utilities/Debug.h"
#include "../Utilities/Exceptions.h"
#include "GameStateStack.h"
#include "PauseMenuState.h"
#include "../Utilities/Exceptions.h"

#include <SDL_video.h>
#include <SDL_mouse.h>
#include <SDL_rect.h>
#include <SDL_mixer.h>

#include <gsl/pointers>
#include <gsl/assert>

#include <array>
#include <algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <cstdint>

namespace ttt::gs
{
	enum class Direction
	{
		up, down, left, right
	};

	GameplayState::GameplayState(Game &game, GameMode mode)
		: game{game},
		mode{mode}
	{
		game.textureCache.load("Resources/GameSheet.png", game.getRenderer());

		game.audioCache.loadChunk("Resources/Player1Move.wav");
		game.audioCache.loadChunk("Resources/Player2Move.wav");
		game.audioCache.loadChunk("Resources/Win.wav");
		game.audioCache.loadChunk("Resources/Lose.wav");

		if (mode == GameMode::vsBot)
		{
			std::ifstream stream{"Resources/Score.sav", std::ifstream::in | std::ifstream::binary};
			if (stream)
			{
				stream.read(reinterpret_cast<char *>(&score1), sizeof(score1));
				stream.read(reinterpret_cast<char *>(&score2), sizeof(score2));
			}
		}

		scoreText1.setText(std::to_string(score1), game.fontCache[2], player1Colour, true, game.getRenderer());
		scoreText2.setText(std::to_string(score2), game.fontCache[2], player2Colour, true, game.getRenderer());

		SDL_Point windowSize;
		SDL_GetWindowSize(game.getWindow(), &windowSize.x, &windowSize.y);

		board.pos.x = windowSize.x / 2 - (board.cellSize.x * Board::size) / 2;
		board.pos.y = 16.0f;

		panelDst.w = static_cast<float>(windowSize.x);
		panelDst.h = 128.0f;
		panelDst.y = windowSize.y - panelDst.h;
	}

	void GameplayState::update()
	{
		const bool winner{lineCheck() || crossCheck()};
		const bool tie{!winner && fullCheck()};

		if (!winner && !tie)
		{
			if (turn == Turn::player1)
				stateText.setText(mode == GameMode::vsBot ? "Your Turn" : "Player 1's Turn", game.fontCache[2], player1Colour, true, game.getRenderer());
			else
				stateText.setText(mode == GameMode::vsBot ? "Bot's Turn" : "Player 2's Turn", game.fontCache[2], player2Colour, true, game.getRenderer());

			switch (mode)
			{
			case GameMode::vsPlayer:
				getInput();
				break;

			case GameMode::vsBot:
			{
				switch (turn)
				{
				case Turn::player1:
					getInput();
					break;

				case Turn::player2:
					board.preview = CellState::empty;

#ifdef TTT_DEBUG
					if (botData->delayTimer == botData->delayTime)
						TTT_CLOG("[BOT] Wasting time...");
#endif
					botData->delayTimer -= 1000.0f / Game::updatesPerSecond;
					if (botData->delayTimer > 0.0f)
						break;
					botData->delayTimer = botData->delayTime;

					const std::uniform_int_distribution<int> typeDist{0, 100};
					// The BEST AI you've ever seen...

					bool played{false};
					if (!board.isEmpty())
					{
						if (typeDist(botData->randEngine) >= 65)
						{
							TTT_CLOG("[BOT] Trying to counter last move....");
							played = counter(botData->lastPlayerMove);
#ifdef TTT_DEBUG
							if (!played)
								std::clog << "[BOT] Failed!\n";
#endif
						}

						if (!played)
							if (typeDist(botData->randEngine) >= 50)
							{
								TTT_CLOG("[BOT] Trying to place counter a random move...");

								SDL_Point cell{getRandomCell()};
								while (!played)
								{
									while (board.getCellState(cell) != CellState::x)
										cell = getRandomCell();
									TTT_CLOG("[BOT] Found cell: " << cell.x << ", " << cell.y << '.');

									played = counter(cell);
									if (!played)
									{
										TTT_CLOG("[BOT] Failed!");
										cell = getRandomCell();
									}
								}
							}
					}

					if (!played)
					{
						TTT_CLOG("[BOT] Making a random move...");

						SDL_Point cell{getRandomCell()};
						while (board.getCellState(cell) != CellState::empty)
							cell = getRandomCell();

						board.setCellState(cell, CellState::o);
					}

					TTT_CLOG("[BOT] Played!");
					Mix_PlayChannel(-1, game.audioCache.getChunk(2), 0);
					turn = Turn::player1;
					break;
				}

				break;
			}
			}
		}
		else
		{
			gameOverTimer -= 1000.0f / Game::updatesPerSecond;
			if (gameOverTimer <= 0.0f)
			{
				gameOverTimer = gameOverTime;
				board.clear();
				gameOverDone = false;
				turn = static_cast<Turn>(randDevice() % 2);
			}
			else
			{
				if (!gameOverDone)
				{
					if (!tie)
					{
						if (turn == Turn::player1)
						{
							++score2;
							stateText.setText(mode == GameMode::vsBot ? "You Lost!" : "Player 2 Won!", game.fontCache[2], player2Colour, true, game.getRenderer());
							scoreText2.setText(std::to_string(score2), game.fontCache[2], player2Colour, true, game.getRenderer());

							if (mode == GameMode::vsBot)
								Mix_PlayChannel(-1, game.audioCache.getChunk(4), 0);
							else
								Mix_PlayChannel(-1, game.audioCache.getChunk(3), 0);

							TTT_CLOG("Player 2 won!");
						}
						else
						{
							++score1;
							stateText.setText(mode == GameMode::vsBot ? "You Won!" : "Player 1 Won!", game.fontCache[2], player1Colour, true, game.getRenderer());
							scoreText1.setText(std::to_string(score1), game.fontCache[2], player1Colour, true, game.getRenderer());

							Mix_PlayChannel(-1, game.audioCache.getChunk(3), 0);
							TTT_CLOG("Player 1 won!");
						}

						if (mode == GameMode::vsBot)
						{
							std::ofstream stream{"Resources/Score.sav", std::ofstream::out | std::ofstream::trunc | std::ofstream::binary};
							if (!stream)
								throw util::IoError{"Could not open save file for writing."};

							stream.write(reinterpret_cast<const char *>(&score1), sizeof(score1));
							stream.write(reinterpret_cast<const char *>(&score2), sizeof(score2));
						}
					}
					else
					{
						stateText.setText("Tie!", game.fontCache[2], {255, 100, 100, 255}, true, game.getRenderer());
						Mix_PlayChannel(-1, game.audioCache.getChunk(4), 0);
						TTT_CLOG("Tie!");
					}

					gameOverDone = true;
				}
			}
		}

		board.update();
		pause.update();
		if (pause.wasReleased())
			game.gameStateStack.emplace<PauseMenuState>(game);
	}

	void GameplayState::render() const noexcept
	{
		const gsl::not_null<SDL_Renderer *> renderer{game.getRenderer()};

		SDL_SetRenderDrawColor(renderer, 150, 50, 50, 255);
		SDL_RenderClear(renderer);

		board.render();

		SDL_RenderCopyF(renderer, game.textureCache[0], &panelSrc, &panelDst);
		stateText.render(renderer, {panelDst.x + panelDst.w / 2 - stateText.getSize().x / 2, panelDst.y + panelDst.h / 2 - stateText.getSize().y / 2});

		scoreText1.render(renderer, {panelDst.x + 24, panelDst.y + panelDst.h / 2 - scoreText1.getSize().y / 2});
		scoreText2.render(renderer, {panelDst.x + panelDst.w - scoreText2.getSize().x - 24, panelDst.y + panelDst.h / 2 - scoreText2.getSize().y / 2});

		score1Sub.render(renderer, {panelDst.x + 24, panelDst.y + panelDst.h / 2 - score1Sub.getSize().y / 2 + scoreText1.getSize().y - 16});
		score2Sub.render(renderer, {panelDst.x + panelDst.w - score2Sub.getSize().x - 24, panelDst.y + panelDst.h / 2 - score2Sub.getSize().y / 2 + scoreText2.getSize().y - 16});

		pause.render(renderer);
	}

	bool GameplayState::counter(const SDL_Point &cell) noexcept
	{
		Expects(cell.x >= 0 && cell.x < Board::size &&cell.y >= 0 && cell.y < Board::size);

		std::array<Direction, 4> directions{Direction::up, Direction::down, Direction::left, Direction::right};
		std::shuffle(directions.begin(), directions.end(), randDevice);

		for (Direction dir : directions)
		{
			switch (dir)
			{
			case Direction::up:
				if (cell.y - 1 >= 0 && board.getCellState({cell.x, cell.y - 1}) == CellState::empty)
				{
					board.setCellState({cell.x, cell.y - 1}, CellState::o);
					return true;
				}
				break;

			case Direction::down:
				if (cell.y + 1 < Board::size && board.getCellState({cell.x, cell.y + 1}) == CellState::empty)
				{
					board.setCellState({cell.x, cell.y + 1}, CellState::o);
					return true;
				}
				break;

			case Direction::left:
				if (cell.x - 1 >= 0 && board.getCellState({cell.x - 1, cell.y}) == CellState::empty)
				{
					board.setCellState({cell.x - 1, cell.y}, CellState::o);
					return true;
				}
				break;

			case Direction::right:
				if (cell.x + 1 < Board::size && board.getCellState({cell.x + 1, cell.y}) == CellState::empty)
				{
					board.setCellState({cell.x + 1, cell.y}, CellState::o);
					return true;
				}
				break;
			}
		}

		return false;
	}

	bool GameplayState::lineCheck(LineType type) const noexcept
	{
		for (int i{0}; i < Board::size; ++i)
		{
			const CellState first{type == LineType::horizontal ? board.getCellState({0, i}) : board.getCellState({i, 0})};
			if (first == CellState::empty)
				continue;

			bool same{true};
			for (int j{1}; j < Board::size; ++j)
				if (board.getCellState(type == LineType::horizontal ? SDL_Point{j, i} : SDL_Point{i, j}) != first)
					same = false;
			if (same)
				return true;
		}

		return false;
	}

	bool GameplayState::crossCheck(CrossType type) const noexcept
	{
		const CellState first{board.getCellState({0, (type == CrossType::leftToRight ? 0 : Board::size - 1)})};
		if (first == CellState::empty)
			return false;

		switch (type)
		{
		case CrossType::leftToRight:
			for (int i{0}; i < 3; ++i)
				if (first != board.getCellState({i, i}))
					return false;
			break;

		case CrossType::rightToLeft:
			for (int x{Board::size - 1}, y{0}; y < Board::size; --x, ++y)
				if (first != board.getCellState({x, y}))
					return false;
			break;
		}

		return true;
	}

	void GameplayState::getInput() noexcept
	{
		SDL_Point mouse;
		const std::uint32_t button{SDL_GetMouseState(&mouse.x, &mouse.y)};

		std::optional<SDL_Point> cell{board.getCell(mouse)};
		if (!cell) return;

		const CellState state{turn == Turn::player1 ? CellState::x : CellState::o};
		board.previewCell = *cell;
		board.preview = state;;

		if ((button & SDL_BUTTON_LMASK) != 0 && board.getCellState(*cell) == CellState::empty)
		{
			board.setCellState(*cell, state);
			if (botData)
				botData->lastPlayerMove = *cell;

			switch (turn)
			{
			case Turn::player1:
				turn = Turn::player2;
				Mix_PlayChannel(-1, game.audioCache.getChunk(1), 0);
				return;

			case Turn::player2:
				turn = Turn::player1;
				Mix_PlayChannel(-1, game.audioCache.getChunk(2), 0);
				return;
			}
		}
	}

	bool GameplayState::fullCheck() const noexcept
	{
		for (int y{0}; y < Board::size; ++y)
			for (int x{0}; x < Board::size; ++x)
				if (board.getCellState({x, y}) == CellState::empty)
					return false;
		return true;
	}
}