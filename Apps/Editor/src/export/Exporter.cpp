#pragma once

#include <osrpch.h>

#include "Exporter.h"

void Exporter::UIntHelper(std::ofstream& stream, uint32_t val)
{
	stream.write((char*)&val, sizeof(uint32_t));
}

void Exporter::StringHelper(std::ofstream& stream, std::string& str)
{
	size_t length = str.length();
	stream.write((char*)&length, sizeof(size_t));
	stream.write(str.c_str(), sizeof(char) * str.length());
}