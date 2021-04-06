#pragma once

#include <osrpch.h>

#include "import/Importer.h"

uint32_t Importer::UIntHelper(std::ifstream& stream)
{
	uint32_t val;
	stream.read((char*)&val, sizeof(uint32_t));
	return val;
}

std::string Importer::StringHelper(std::ifstream& stream)
{
	std::string str;
	size_t size;
	stream.read((char*)&size, sizeof(size_t));
	str.resize(size);
	stream.read(&str[0], size);
	return str;
}