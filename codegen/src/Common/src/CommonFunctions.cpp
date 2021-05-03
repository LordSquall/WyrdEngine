#pragma once

#include "CommonFunctions.h"

/* int8_t */
void Read(std::ifstream& s, int8_t& d)
{
    s.read((char*)&d, sizeof(int8_t));
}

void Write(std::ofstream& s, int8_t d)
{
    s.write((char*)&d, sizeof(int8_t));
}

/* int16_t */
void Read(std::ifstream& s, int16_t& d)
{
    s.read((char*)&d, sizeof(int16_t));
}

void Write(std::ofstream& s, int16_t d)
{
    s.write((char*)&d, sizeof(int16_t));
}

/* int32_t */
void Read(std::ifstream& s, int32_t& d)
{
    s.read((char*)&d, sizeof(int32_t));
}

void Write(std::ofstream& s, int32_t d)
{
    s.write((char*)&d, sizeof(int32_t));
}

/* int64_t */
void Read(std::ifstream& s, int64_t& d)
{
    s.read((char*)&d, sizeof(int64_t));
}

void Write(std::ofstream& s, int64_t d)
{
    s.write((char*)&d, sizeof(int64_t));
}

/* uint8_t */
void Read(std::ifstream& s, uint8_t& d)
{
    s.read((char*)&d, sizeof(uint8_t));
}

void Write(std::ofstream& s, uint8_t d)
{
    s.write((char*)&d, sizeof(uint8_t));
}

/* uint16_t */
void Read(std::ifstream& s, uint16_t& d)
{
    s.read((char*)&d, sizeof(uint16_t));
}

void Write(std::ofstream& s, uint16_t d)
{
    s.write((char*)&d, sizeof(uint16_t));
}

/* uint32_t */
void Read(std::ifstream& s, uint32_t& d)
{
    s.read((char*)&d, sizeof(uint32_t));
}

void Write(std::ofstream& s, uint32_t d)
{
    s.write((char*)&d, sizeof(uint32_t));
}

/* uint64_t */
void Read(std::ifstream& s, uint64_t& d)
{
    s.read((char*)&d, sizeof(uint64_t));
}

void Write(std::ofstream& s, uint64_t d)
{
    s.write((char*)&d, sizeof(uint64_t));
}

/* float */
void Read(std::ifstream& s, float& d)
{
    s.read((char*)&d, sizeof(float));
}

void Write(std::ofstream& s, float d)
{
    s.write((char*)&d, sizeof(float));
}

/* char */
void Read(std::ifstream& s, char& d)
{
    s.read((char*)&d, sizeof(char));
}

void Write(std::ofstream& s, char d)
{
    s.write((char*)&d, sizeof(char));
}

/* std::string */
void Read(std::ifstream& s, std::string& d)
{
    uint32_t size;
    s.read((char*)&size, sizeof(uint32_t));
    d.resize(size);
    s.read((char*)&d[0], sizeof(char) * size);
}

void Write(std::ofstream& s, std::string& d)
{
    uint32_t size = (uint32_t)d.length();
    s.write((char*)&size, sizeof(uint32_t));
    s.write(d.c_str(), sizeof(char) * size);
}

/* Wyrd::UID */
void Read(std::ifstream& s, Wyrd::UID& uid)
{
    std::string str;
    uint32_t size;
    s.read((char*)&size, sizeof(uint32_t));
    str.resize(size);
    s.read((char*)&str[0], sizeof(char) * size);
    uid = Wyrd::UID(str.c_str());
}

void Write(std::ofstream& s, Wyrd::UID& uid)
{
    std::string uidStr = uid.str();
    uint32_t size = (uint32_t)uidStr.length();
    s.write((char*)&size, sizeof(uint32_t));
    s.write(uidStr.c_str(), sizeof(char) * size);
}
