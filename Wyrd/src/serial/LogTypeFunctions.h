#pragma once

#include <type_traits>
#include <spdlog/spdlog.h>

#include <core/UID.h>

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<Wyrd::UID, T>::value, char>> : fmt::formatter<std::string> {
    auto format(const Wyrd::UID& a, format_context& ctx) const {
        return fmt::formatter<std::string>::format(a.str(), ctx);
    }
};

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<std::filesystem::path, T>::value, char>> : fmt::formatter<std::string> {
    auto format(const std::filesystem::path& a, format_context& ctx) const {
        return fmt::formatter<std::string>::format(a.string(), ctx);
    }
};