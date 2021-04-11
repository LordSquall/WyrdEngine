/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

struct BundleFormat_Config
{
    std::string version;
};

struct BundleFormat_Shader
{
    std::string vertexSrc;
    std::string fragmentSrc;
};

struct BundleFormat_ShaderConfig
{
    uint32_t count;
    uint32_t shaders_cnt;
    std::vector<Shader> shaders;
};

struct BundleFormat_Texture
{
    uint32_t width;
    uint32_t height;
    uint32_t channels;
};

struct BundleFormat_TextureConfig
{
    uint32_t count;
    uint32_t textures_cnt;
    std::vector<Texture> textures;
};

