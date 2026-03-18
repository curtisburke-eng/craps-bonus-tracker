
#pragma once

#include <ostream>
#include <system_error>
#include <spdlog/spdlog.h>

enum class CbtErrorCodes
{
    eOk,
    eDatabaseConnected,
    eDatabaseDisconnected,
    eQueryFailed,
    ePlayerNotFound,
    ePlayerActive,
    eGameActive,
    eGameNotActive,
    eInvalidArgument,
    eUnsupported,
    eUnknown
};

namespace std {
    // Tell the compliler this enum should be treated as an error code enum
    template<>
    struct is_error_code_enum<CbtErrorCodes> : true_type {};
}

class CbtErrorCategory : public std::error_category
{
public:
    const char* name() const noexcept override
    {
        return "CBT";
    }

    std::string message(int value) const override
    {
        switch (static_cast<CbtErrorCodes>(value))
        {
        case CbtErrorCodes::eOk:
            return "Ok";
        case CbtErrorCodes::eDatabaseConnected:
            return "Database is already connected";
        case CbtErrorCodes::eDatabaseDisconnected:
            return "Database disconnected";
        case CbtErrorCodes::eQueryFailed:
            return "Database Query Failed";
        case CbtErrorCodes::ePlayerNotFound:
            return "Player not found";
        case CbtErrorCodes::ePlayerActive:
            return "Player already active";
        case CbtErrorCodes::eGameActive:
            return "Game already active";
        case CbtErrorCodes::eGameNotActive:
            return "Game not active";
        case CbtErrorCodes::eInvalidArgument:
            return "Invalid argument";
        case CbtErrorCodes::eUnsupported:
            return "Feature not supported";
        case CbtErrorCodes::eUnknown:
            return "Unknown error";
        default:
            return "CBT error";
        }
    }
};

/**
 * @brief Get the singleton instance of the CbtErrorCodes Category.
 */
inline const std::error_category& GetCbtErrorCategory()
{
    static CbtErrorCategory instance;
    return instance;
}

/**
 * @brief Create an error code from a CbtErrorCodes value.
 */
inline std::error_code make_error_code(CbtErrorCodes e)
{
    return {static_cast<int>(e), GetCbtErrorCategory()};
}

// PrintTo for GTest ADL lookup - print the human-readable message instead of the CbtErrorCodes enum int value on failures.
inline void PrintTo(CbtErrorCodes e, std::ostream* os)
{
    *os << make_error_code(e).message() << " (" << static_cast<int>(e) << ")";
}

// for GTest's ADL lookup - Overload PrintTo in std namespace due to std::error_code in std namespace
namespace std
{
    inline void PrintTo(const error_code& ec, ostream* os)
    {
        *os << ec.category().name() << "::" << ec.message() << " (" << ec.value() << ")";
    }
}
