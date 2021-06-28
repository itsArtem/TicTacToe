#ifndef TTT_UTIL_UTILITIES_H
#define TTT_UTIL_UTILITIES_H

#include <gsl/util>

namespace ttt::util
{
	// Used to avoid having to cast std::size_t to gsl::index.
	template<typename Container>
	inline gsl::index getSize(const Container &container) noexcept
	{
		return gsl::narrow_cast<gsl::index>(container.size());
	}
}

#endif