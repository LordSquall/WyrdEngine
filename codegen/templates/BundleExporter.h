/// Auto Generated
/// Date: Apr 2021 Sun 10:20:05

#pragma once

#include <osrpch.h>

class BundleFormatExporter
{
    static void Write(std::ofstream& s, BundleFormat_Config& d);
    static void Write(std::ofstream& s, BundleFormat_Shader& d);
    static void Write(std::ofstream& s, BundleFormat_ShaderConfig& d);
    static void Write(std::ofstream& s, BundleFormat_Texture& d);
    static void Write(std::ofstream& s, BundleFormat_TextureConfig& d);
};
