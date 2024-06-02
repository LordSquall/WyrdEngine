#pragma once

#include <type_traits>
#include <spdlog/spdlog.h>

#include "loaders/ResourceSetLoader.h"

template <typename T>
struct fmt::formatter<T, std::enable_if_t<std::is_base_of<Wyrd::Editor::ResourceSetLoader::Result, T>::value, char>> : fmt::formatter<std::string> {
    auto format(const Wyrd::Editor::ResourceSetLoader::Result& a, format_context& ctx) const {
        return fmt::formatter<std::string>::format(a, ctx);
    }
};