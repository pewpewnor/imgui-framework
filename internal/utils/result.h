#pragma once

#include <expected>
#include <optional>
#include <string>

template <typename T>
using Result = std::expected<T, std::string>;

using Fallible = std::optional<std::string>;
