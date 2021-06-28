#include "Board.h"
#include "Game.h"
#include "Utilities/Utilities.h"

#include <SDL_video.h>

#include <gsl/pointers>

namespace ttt
{
	Board::Board(const SDL_FPoint &pos, const SDL_FPoint &cellSize, const Game &game) noexcept
		: pos{pos},
		cellSize{cellSize},
		game{game}
	{
	}

	void Board::update() noexcept
	{
		constexpr float offsetInc{-0.5f};

		if (shrinkPreview)
			previewOffset -= offsetInc;
		else
			previewOffset += offsetInc;
		if (previewOffset <= -8.0f || previewOffset >= 2.0f)
			shrinkPreview = !shrinkPreview;
	}

	void Board::render() const noexcept
	{
		const gsl::not_null<SDL_Renderer *> renderer{game.getRenderer()};
		SDL_SetTextureAlphaMod(game.textureCache[1], 255);

		for (int i{0}; i < 2; ++i)
		{
			const SDL_FRect dst{pos.x + cellSize.x + cellSize.x * i, pos.y, lineWidth, cellSize.x * size + lineWidth / 4};
			SDL_RenderCopyF(renderer, game.textureCache[1], &lineSrc, &dst);
		}
		for (int i{0}; i < 2; ++i)
		{
			const SDL_FRect dst{pos.x, pos.y + cellSize.y + cellSize.y * i, lineWidth, cellSize.y * size + lineWidth / 2};
			constexpr SDL_FPoint centre{lineWidth / 2, lineWidth / 2};
			SDL_RenderCopyExF(renderer, game.textureCache[1], &lineSrc, &dst, 270.0, &centre, SDL_FLIP_HORIZONTAL);
		}

		for (int x{0}; x < size; ++x)
			for (int y{0}; y < size; ++y)
			{
				SDL_FRect dst{
					((pos.x + cellSize.x * x + lineWidth / 2) + cellSize.x / 2 - (cellSize.y * 0.7f) / 2),
					((pos.y + cellSize.y * y + lineWidth / 2) + cellSize.y / 2 - (cellSize.y * 0.7f) / 2),
					cellSize.x * 0.7f,
					cellSize.y * 0.7f
				};

				const CellState state{gsl::at(grid, x + y * size)};
				switch (state)
				{
				case CellState::empty:
					if (preview != CellState::empty && previewCell.x == x && previewCell.y == y)
					{
						SDL_SetTextureAlphaMod(game.textureCache[1], 100);

						dst.x -= previewOffset;
						dst.y -= previewOffset;
						dst.w += previewOffset * 2;
						dst.h += previewOffset * 2;

						switch (preview)
						{
						case CellState::x:
							SDL_RenderCopyF(renderer, game.textureCache[1], &xSrc, &dst);
							break;

						case CellState::o:
							SDL_RenderCopyF(renderer, game.textureCache[1], &oSrc, &dst);
							break;
						}
					}
					break;

				case CellState::x:
					SDL_SetTextureAlphaMod(game.textureCache[1], 255);
					SDL_RenderCopyF(renderer, game.textureCache[1], &xSrc, &dst);
					break;

				case CellState::o:
					SDL_SetTextureAlphaMod(game.textureCache[1], 255);
					SDL_RenderCopyF(renderer, game.textureCache[1], &oSrc, &dst);
					break;
				}
			}
	}

	std::optional<SDL_Point> Board::getCell(const SDL_Point &point) const noexcept
	{
		if (point.x < pos.x || point.x >= pos.x + cellSize.x * size || point.y < pos.y || point.y >= pos.y + cellSize.y * size)
			return std::nullopt;

		return SDL_Point{static_cast<int>((point.x - pos.x - lineWidth / 2) / cellSize.x), static_cast<int>((point.y - pos.y - lineWidth / 2) / cellSize.y)};
	}

	void Board::clear() noexcept
	{
		for (int y{0}; y < size; ++y)
			for (int x{0}; x < size; ++x)
				gsl::at(grid, x + y * size) = CellState::empty;
	}

	bool Board::isEmpty() const noexcept
	{
		for (int y{0}; y < size; ++y)
			for (int x{0}; x < size; ++x)
				if (gsl::at(grid, x + y * size) != CellState::empty)
					return false;
		return true;
	}
}