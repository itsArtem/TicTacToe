#ifndef TTT_BOARD_H
#define TTT_BOARD_H

#include <SDL_rect.h>

#include <gsl/util>

#include <array>
#include <optional>

namespace ttt
{
	class Game;

	enum class CellState
	{
		empty, x, o
	};

	class Board final
	{
	public:
		SDL_FPoint pos;
		SDL_FPoint cellSize;
		static constexpr int size{3};

		CellState preview{CellState::empty};
		SDL_Point previewCell{};

		Board(const SDL_FPoint &pos, const SDL_FPoint &cellSize, const Game &game) noexcept;

		void update() noexcept;
		void render() const noexcept;

		[[nodiscard]] CellState getCellState(const SDL_Point &pos) const noexcept;
		void setCellState(const SDL_Point &pos, CellState state) noexcept;
		void clear() noexcept;

		[[nodiscard]] std::optional<SDL_Point> getCell(const SDL_Point &point) const noexcept;
		[[nodiscard]] bool isEmpty() const noexcept;

	private:
		const Game &game;
		std::array<CellState, 9> grid{};

		static constexpr float lineWidth{16.0f};
		float previewOffset{0.0f};
		bool shrinkPreview{false};

		const SDL_Rect lineSrc{0, 0, 8, 576};
		const SDL_Rect xSrc{8, 0, 48, 48};
		const SDL_Rect oSrc{8, 48, 48, 48};
	};

	inline CellState Board::getCellState(const SDL_Point &pos) const noexcept
	{
		return gsl::at(grid, pos.x + pos.y * size);
	}

	inline void Board::setCellState(const SDL_Point &pos, CellState state) noexcept 
	{
		gsl::at(grid, pos.x + pos.y * size) = state;
	}
}

#endif