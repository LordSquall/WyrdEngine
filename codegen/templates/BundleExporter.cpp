/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

void Write(std::ofstream& s, BundleFormat_Config& d);
{
    Write(s, d.version);
};

void Write(std::ofstream& s, BundleFormat_Shader& d);
{
    Write(s, d.vertexSrc);
    Write(s, d.fragmentSrc);
};

void Write(std::ofstream& s, BundleFormat_ShaderConfig& d);
{
    Write(s, d.count);
    Write(s, d.shaders_cnt);
    for (uint32_t i = 0; i < d.shaders_cnt; i++)
    {
        Write(s, d.shaders[i]);
    }
};

void Write(std::ofstream& s, BundleFormat_Texture& d);
{
    Write(s, d.width);
    Write(s, d.height);
    Write(s, d.channels);
};

void Write(std::ofstream& s, BundleFormat_TextureConfig& d);
{
    Write(s, d.count);
    Write(s, d.textures_cnt);
    for (uint32_t i = 0; i < d.textures_cnt; i++)
    {
        Write(s, d.textures[i]);
    }
};

