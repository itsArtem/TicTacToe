#include "Game.h"
#include "Utilities/Exceptions.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <iostream>
#include <exception>
#include <string>
#include <cstdlib>

namespace ttt
{
	namespace
	{
		void init()
		{
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0
				|| IMG_Init(IMG_INIT_PNG) == 0
				|| TTF_Init() == -1
				|| Mix_Init(MIX_INIT_MID) == 0 || Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
			{
				throw util::ExternalError{SDL_GetError()};
			}
		}

		void quit() noexcept
		{
			SDL_Quit();
			IMG_Quit();
			TTF_Quit();
			Mix_CloseAudio();
			Mix_Quit();
		}
	}
}

int main(int argc, char *argv[])
{
	int status{EXIT_SUCCESS};
	ttt::init();

	try
	{
		ttt::Game{}.run();
	}
	catch (std::exception &e)
	{
		using namespace std::string_literals;

		std::cerr << "CRASH! " << e.what() << '\n';
		SDL_ShowSimpleMessageBox(SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR,
			"CRASH!",
			("Looks like the game has crashed. Sorry about that!\n"
				"Make sure there are no missing game files, or perhaps try not doing what you were doing...\n"
				"You can open an issue at github.com/itsArtem/TicTacToe.\n\n"
				"what():\n"s + e.what()).c_str(),
			nullptr);

		status = EXIT_FAILURE;
	}

	ttt::quit();
	return status;
}