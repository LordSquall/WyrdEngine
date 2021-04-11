#pragma once

#include "CommonImports.h"

void Read(std::ifstream& s, int8_t& d)
{
	s.read((char*)&d, sizeof(int8_t));
}

void Read(std::ifstream& s, int16_t& d)
{
	s.read((char*)&d, sizeof(int16_t));
}

void Read(std::ifstream& s, int32_t& d)
{
	s.read((char*)&d, sizeof(int32_t));
}

void Read(std::ifstream& s, int64_t& d)
{
	s.read((char*)&d, sizeof(int64_t));
}

void Read(std::ifstream& s, uint8_t& d)
{
	s.read((char*)&d, sizeof(uint8_t));
}

void Read(std::ifstream& s, uint16_t& d)
{
	s.read((char*)&d, sizeof(uint16_t));
}

void Read(std::ifstream& s, uint32_t& d)
{
	s.read((char*)&d, sizeof(uint32_t));
}

void Read(std::ifstream& s, uint64_t& d)
{
	s.read((char*)&d, sizeof(uint64_t));
}

void Read(std::ifstream& s, std::string& d)
{
	uint32_t size;
	s.read((char*)&size, sizeof(uint32_t));
	d.resize(size);
	s.read((char*)&d[0], sizeof(char) * size);
}

void Read(std::ifstream& s, Osiris::UID& uid)
{
	std::string str;
	uint32_t size;
	s.read((char*)&size, sizeof(uint32_t));
	str.resize(size);
	s.read((char*)&str[0], sizeof(char) * size);
	uid = Osiris::UID(str.c_str());
}