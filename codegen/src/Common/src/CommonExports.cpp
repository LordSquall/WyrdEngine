#pragma once

#include "CommonExports.h"

void Write(std::ofstream& s, int8_t d)
{
    s.write((char*)&d, sizeof(int8_t));
}

void Write(std::ofstream& s, int16_t d)
{
    s.write((char*)&d, sizeof(int16_t));
}

void Write(std::ofstream& s, int32_t d)
{
    s.write((char*)&d, sizeof(int32_t));
}

void Write(std::ofstream& s, int64_t d)
{
    s.write((char*)&d, sizeof(int64_t));
}

void Write(std::ofstream& s, uint8_t d)
{
    s.write((char*)&d, sizeof(uint8_t));
}

void Write(std::ofstream& s, uint16_t d)
{
    s.write((char*)&d, sizeof(uint16_t));
}

void Write(std::ofstream& s, uint32_t d)
{
    s.write((char*)&d, sizeof(uint32_t));
}

void Write(std::ofstream& s, uint64_t d)
{
    s.write((char*)&d, sizeof(uint64_t));
}

void Write(std::ofstream& s, std::string& d)
{
    uint32_t size = (uint32_t)d.length();
    s.write((char*)&size, sizeof(uint32_t));
    s.write(d.c_str(), sizeof(char) * size);
}

void Write(std::ofstream& s, Osiris::UID& uid)
{
    std::string uidStr = uid.str();
    uint32_t size = (uint32_t)uidStr.length();
    s.write((char*)&size, sizeof(uint32_t));
    s.write(uidStr.c_str(), sizeof(char) * size);
}