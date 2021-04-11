/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

class BundleFormatImporter
{
    static void Read(std::ifstream& s, BundleFormat_Config& d);
    static void Read(std::ifstream& s, BundleFormat_Shader& d);
    static void Read(std::ifstream& s, BundleFormat_ShaderConfig& d);
    static void Read(std::ifstream& s, BundleFormat_Texture& d);
    static void Read(std::ifstream& s, BundleFormat_TextureConfig& d);
};
