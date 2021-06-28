#ifndef TTT_UTIL_EXCEPTIONS_H
#define TTT_UTIL_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace ttt::util
{
	// For errors caused by external code.
	class ExternalError final : public std::runtime_error
	{
	public:
		ExternalError(const std::string &msg) noexcept;
	};

	inline ExternalError::ExternalError(const std::string &msg) noexcept
		: runtime_error{msg} { }

	// For file and other IO-related errors.
	class IoError final : public std::runtime_error
	{
	public:
		IoError(const std::string &msg) noexcept;
	};

	inline IoError::IoError(const std::string &msg) noexcept
		: runtime_error{msg} { }
}

#endif