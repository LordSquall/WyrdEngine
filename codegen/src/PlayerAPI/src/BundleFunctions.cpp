/// Auto Generated
/// Date: Apr 2021 Sun 21:39:54

#pragma once

#include <osrpch.h>

#include "CommonFunctions.h"

#include "BundleFormat.h"

void Read(std::ifstream& s, BundleFormat_Config& d)
{
    Read(s, d.version);
};

void Write(std::ofstream& s, BundleFormat_Config& d)
{
    Write(s, d.version);
};

void Read(std::ifstream& s, BundleFormat_Shader& d)
{
    Read(s, d.uid);
    Read(s, d.vertexSrc);
    Read(s, d.fragmentSrc);
};

void Write(std::ofstream& s, BundleFormat_Shader& d)
{
    Write(s, d.uid);
    Write(s, d.vertexSrc);
    Write(s, d.fragmentSrc);
};

void Read(std::ifstream& s, BundleFormat_ShaderConfig& d)
{
    Read(s, d.shaders_cnt);
    for (uint32_t i = 0; i < d.shaders_cnt; i++)
    {
        BundleFormat_Shader item;
        Read(s, item);
        d.shaders.push_back(item);
    }
};

void Write(std::ofstream& s, BundleFormat_ShaderConfig& d)
{
    Write(s, d.shaders_cnt);
    for (uint32_t i = 0; i < d.shaders_cnt; i++)
    {
        Write(s, d.shaders[i]);
    }
};

void Read(std::ifstream& s, BundleFormat_Texture& d)
{
    Read(s, d.uid);
    Read(s, d.width);
    Read(s, d.height);
    Read(s, d.channels);
    Read(s, d.data_cnt);
    for (uint32_t i = 0; i < d.data_cnt; i++)
    {
        unsigned char item;
        Read(s, item);
        d.data.push_back(item);
    }
};

void Write(std::ofstream& s, BundleFormat_Texture& d)
{
    Write(s, d.uid);
    Write(s, d.width);
    Write(s, d.height);
    Write(s, d.channels);
    Write(s, d.data_cnt);
    for (uint32_t i = 0; i < d.data_cnt; i++)
    {
        Write(s, d.data[i]);
    }
};

void Read(std::ifstream& s, BundleFormat_TextureConfig& d)
{
    Read(s, d.textures_cnt);
    for (uint32_t i = 0; i < d.textures_cnt; i++)
    {
        BundleFormat_Texture item;
        Read(s, item);
        d.textures.push_back(item);
    }
};

void Write(std::ofstream& s, BundleFormat_TextureConfig& d)
{
    Write(s, d.textures_cnt);
    for (uint32_t i = 0; i < d.textures_cnt; i++)
    {
        Write(s, d.textures[i]);
    }
};

