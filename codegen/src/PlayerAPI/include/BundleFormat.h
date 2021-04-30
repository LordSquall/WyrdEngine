/// Auto Generated
/// Date: Apr 2021 Thu 22:14:39

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
    Osiris::UID uid;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint32_t data_cnt;
    std::vector<char> data;
};

struct BundleFormat_TextureConfig
{
    uint32_t textures_cnt;
    std::vector<BundleFormat_Texture> textures;
};

struct BundleFormat_ManagedLib
{
    std::string name;
    uint32_t data_cnt;
    std::vector<char> data;
};

struct BundleFormat_ManagedLibConfig
{
    uint32_t classes_cnt;
    std::vector<std::string> classes;
    uint32_t classesUIDs_cnt;
    std::vector<Osiris::UID> classesUIDs;
    uint32_t managedLibraries_cnt;
    std::vector<BundleFormat_ManagedLib> managedLibraries;
};
