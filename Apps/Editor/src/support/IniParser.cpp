#pragma once

#include "wyrdpch.h"

#include "IniParser.h"

/* ini file parser for configuration */
#define INI_IMPLEMENTATION
#include "../support/ini.h"

IniParser::IniParser(std::string name)
{
	m_IsLoaded = false;

	FILE* fp = fopen(name.c_str(), "r");
	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char* data = (char*)malloc(size + 1);

		fread(data, 1, size, fp);
		data[size] = '\0';
		fclose(fp);

		m_IniCtx = ini_load(data, NULL);
		free(data);

		if (m_IniCtx != NULL)
			m_IsLoaded = true;
	}
	else
	{
		m_IniCtx = ini_create(NULL);
	}
}

IniParser::~IniParser()
{
	if (m_IsLoaded)
		ini_destroy(m_IniCtx);
}

std::string IniParser::GetValue(std::string section, std::string name, std::string defaultValue)
{
	int sectionIdx = 0;
	int nameIdx = 0;

	if (section.empty() == false)
		sectionIdx = ini_find_section(m_IniCtx, section.c_str(), (int)section.length());

	nameIdx = ini_find_property(m_IniCtx, sectionIdx, name.c_str(), (int)name.length());

	if (nameIdx == -1)
	{
		return defaultValue;
	}
	else
	{
		return ini_property_value(m_IniCtx, sectionIdx, nameIdx);
	}
}

void IniParser::SetValue(std::string value, std::string section, std::string name)
{
	int sectionIdx = 0;
	int nameIdx = 0;

	if (section.empty() == false)
	{
		sectionIdx = ini_find_section(m_IniCtx, section.c_str(), (int)section.length());
		if (sectionIdx == -1)
		{
			sectionIdx = ini_section_add(m_IniCtx, section.c_str(), (int)section.length());
		}
	}

	nameIdx = ini_find_property(m_IniCtx, sectionIdx, name.c_str(), (int)name.length());
	if (nameIdx == -1)
	{
		ini_property_add(m_IniCtx, sectionIdx, name.c_str(), (int)name.length(), value.c_str(), (int)value.length());
	}
	else
	{
		ini_property_value_set(m_IniCtx, sectionIdx, nameIdx, value.c_str(), (int)value.length());
	}
}

void IniParser::Save(std::string name)
{
	int size = ini_save(m_IniCtx, NULL, 0);
	char* data = (char*)malloc(size);
	size = ini_save(m_IniCtx, data, size);

	FILE* fp = fopen(name.c_str(), "w");
	fwrite(data, 1, size, fp);
	fclose(fp);
	free(data);
}