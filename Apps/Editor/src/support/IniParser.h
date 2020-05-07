#pragma once

class ini_t;

class IniParser
{
public:
	IniParser(std::string name);
	~IniParser();

	std::string GetValue(std::string section, std::string name, std::string defaultValue);

	void SetValue(std::string value, std::string section, std::string name);

	void Save(std::string name);

public:
	inline bool IsLoaded() const { return m_IsLoaded; }

private:
	bool	m_IsLoaded;
	ini_t*	m_IniCtx;
};