#pragma once

#ifndef NDEBUG
#include <cassert>

#define ASSERT(condition, message) assert((condition) && (message))
#else
#include <spdlog/spdlog.h>

#include <stdexcept>

#define ASSERT(condition, message)                        \
    do {                                                  \
        if (!(condition)) {                               \
            std::string error_msg = "Assertion Failed: "; \
            error_msg += message;                         \
            spdlog::error("{}", error_msg);               \
            throw std::runtime_error(error_msg);          \
        }                                                 \
    } while (0)
#endif
