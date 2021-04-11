#pragma once

#include <osrpch.h>
#include <core/UID.h>

void Write(std::ofstream& s, int8_t d);
void Write(std::ofstream& s, int16_t d);
void Write(std::ofstream& s, int32_t d);
void Write(std::ofstream& s, int64_t d);

void Write(std::ofstream& s, uint8_t d);
void Write(std::ofstream& s, uint16_t d); 
void Write(std::ofstream& s, uint32_t d);
void Write(std::ofstream& s, uint64_t d);

void Write(std::ofstream& s, Osiris::UID& uid);

void Write(std::ofstream& s, std::string& d);