/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

void Read(std::ifstream& s, BundleFormat_Config& d);
{
    Read(s, d.version);
};

void Read(std::ifstream& s, BundleFormat_Shader& d);
{
    Read(s, d.vertexSrc);
    Read(s, d.fragmentSrc);
};

void Read(std::ifstream& s, BundleFormat_ShaderConfig& d);
{
    Read(s, d.count);
    Read(s, d.shaders_cnt);
    for (uint32_t i = 0; i < d.shaders_cnt; i++)
    {
        BundleFormat_Shader item;
        Read(s, shaders);
        d.shaders.push_back(item);
    }
};

void Read(std::ifstream& s, BundleFormat_Texture& d);
{
    Read(s, d.width);
    Read(s, d.height);
    Read(s, d.channels);
};

void Read(std::ifstream& s, BundleFormat_TextureConfig& d);
{
    Read(s, d.count);
    Read(s, d.textures_cnt);
    for (uint32_t i = 0; i < d.textures_cnt; i++)
    {
        BundleFormat_Texture item;
        Read(s, textures);
        d.textures.push_back(item);
    }
};

