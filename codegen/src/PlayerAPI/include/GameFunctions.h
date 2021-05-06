/// Auto Generated
/// Date: May 2021 Wed 21:51:31

#pragma once

#include <wyrdpch.h>

#include "GameFormat.h"

void Read(std::ifstream& s, GameFormat_Config& d);
void Write(std::ofstream& s, GameFormat_Config& d);

void Read(std::ifstream& s, GameFormat_Display& d);
void Write(std::ofstream& s, GameFormat_Display& d);

void Read(std::ifstream& s, GameFormat_SceneListings& d);
void Write(std::ofstream& s, GameFormat_SceneListings& d);

