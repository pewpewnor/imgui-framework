#pragma once

#ifndef NDEBUG
#include <cassert>

#define ASSERT(condition, message) assert((condition) && (message))
#define ASSERT_SOFT(condition, message) assert((condition) && (message))
#else
#include <spdlog/spdlog.h>

#include <stdexcept>
#include <string>

#define ASSERT(condition, message)                             \
    do {                                                       \
        if (!(condition)) {                                    \
            std::string error_msg = "Hard assertion failed: "; \
            error_msg += message;                              \
            spdlog::error("{}", error_msg);                    \
            throw std::runtime_error(error_msg);               \
        }                                                      \
    } while (0)

#define ASSERT_SOFT(condition, message)                         \
    do {                                                        \
        if (!(condition)) {                                     \
            spdlog::warn("Soft assertion failed: {}", message); \
        }                                                       \
    } while (0)
#endif
