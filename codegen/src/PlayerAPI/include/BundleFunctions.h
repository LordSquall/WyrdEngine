/// Auto Generated
/// Date: May 2021 Wed 21:51:31

#pragma once

#include <wyrdpch.h>

#include "BundleFormat.h"

void Read(std::ifstream& s, BundleFormat_Config& d);
void Write(std::ofstream& s, BundleFormat_Config& d);

void Read(std::ifstream& s, BundleFormat_Shader& d);
void Write(std::ofstream& s, BundleFormat_Shader& d);

void Read(std::ifstream& s, BundleFormat_ShaderConfig& d);
void Write(std::ofstream& s, BundleFormat_ShaderConfig& d);

void Read(std::ifstream& s, BundleFormat_Texture& d);
void Write(std::ofstream& s, BundleFormat_Texture& d);

void Read(std::ifstream& s, BundleFormat_TextureConfig& d);
void Write(std::ofstream& s, BundleFormat_TextureConfig& d);

void Read(std::ifstream& s, BundleFormat_ManagedLib& d);
void Write(std::ofstream& s, BundleFormat_ManagedLib& d);

void Read(std::ifstream& s, BundleFormat_ManagedLibConfig& d);
void Write(std::ofstream& s, BundleFormat_ManagedLibConfig& d);

