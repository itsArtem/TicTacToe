#include "AudioCache.h"
#include "Utilities/Exceptions.h"

namespace ttt
{
	Mix_Music *AudioCache::loadMusic(std::string_view path)
	{
		Mix_Music *const track{Mix_LoadMUS(path.data())};
		if (!track)
			throw util::IoError{SDL_GetError()};
		
		music.emplace_back(track, Mix_FreeMusic);
		return track;
	}
	
	Mix_Chunk *AudioCache::loadChunk(std::string_view path)
	{
		Mix_Chunk *const chunk{Mix_LoadWAV(path.data())};
		if (!chunk)
			throw util::IoError{SDL_GetError()};

		chunks.emplace_back(chunk, Mix_FreeChunk);
		return chunk;
	}

	void AudioCache::clear() noexcept
	{
		music.clear();
		chunks.clear();
	}
}