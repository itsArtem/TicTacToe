#ifndef TTT_AUDIOCACHE_H
#define TTT_AUDIOCACHE_H

#include <SDL_mixer.h>

#include <gsl/util>

#include <vector>
#include <memory>
#include <string_view>

namespace ttt
{
	// A class for caching sounds and music.
	class AudioCache final
	{
	public:
		using SizeType = gsl::index;

		Mix_Music *loadMusic(std::string_view path);
		Mix_Chunk *loadChunk(std::string_view path);

		void unloadMusic(SizeType index) noexcept;
		void unloadChunk(SizeType index) noexcept;
		
		void eraseMusic(SizeType index) noexcept;
		void eraseChunk(SizeType index) noexcept;

		void clearMusic() noexcept;
		void clearChunks() noexcept;
		void clear() noexcept;

		[[nodiscard]] Mix_Music *getMusic(SizeType index) const noexcept;
		[[nodiscard]] Mix_Chunk *getChunk(SizeType index) const noexcept;

	private:
		std::vector<std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>> music;
		std::vector<std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>> chunks;
	};

	inline void AudioCache::unloadMusic(SizeType index) noexcept { gsl::at(music, index).reset(); }
	inline void AudioCache::unloadChunk(SizeType index) noexcept { gsl::at(chunks, index).reset(); }

	inline void AudioCache::eraseMusic(SizeType index) noexcept { music.erase(music.cbegin() + index); }
	inline void AudioCache::eraseChunk(SizeType index) noexcept { chunks.erase(chunks.cbegin() + index); }
	
	inline void AudioCache::clearMusic() noexcept { music.clear(); }
	inline void AudioCache::clearChunks() noexcept { chunks.clear(); }

	inline Mix_Music *AudioCache::getMusic(SizeType index) const noexcept { return gsl::at(music, index).get(); }
	inline Mix_Chunk *AudioCache::getChunk(SizeType index) const noexcept { return gsl::at(chunks, index).get(); }
}

#endif