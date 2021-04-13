/// Auto Generated
/// Date: Apr 2021 Tue 22:10:41

#pragma once

#include <osrpch.h>

#include <core/UID.h>

struct BundleFormat_Config
{
    std::string version;
};

struct BundleFormat_Shader
{
    Osiris::UID uid;
    std::string vertexSrc;
    std::string fragmentSrc;
};

struct BundleFormat_ShaderConfig
{
    uint32_t shaders_cnt;
    std::vector<BundleFormat_Shader> shaders;
};

struct BundleFormat_Texture
{
    uint32_t width;
    uint32_t height;
    uint32_t channels;
};

struct BundleFormat_TextureConfig
{
    uint32_t textures_cnt;
    std::vector<BundleFormat_Texture> textures;
};

