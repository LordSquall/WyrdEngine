#pragma once

#include "core/export.h"
#include "core/Layer.h"

namespace Osiris
{
	class Icon
	{
	public:
		Icon() {}
		virtual ~Icon() {}

		inline unsigned int GetHandle() { return m_Handle; }
		inline unsigned char* GetData() { return m_Data; }
		inline unsigned int GetXPos() { return m_XPos; }
		inline unsigned int GetYPos() { return m_YPos; }
		inline unsigned int GetWidth() { return m_Width; }
		inline unsigned int GetHeight() { return m_Height; }
		inline float* GetUV0() { return &m_UV0[0]; }
		inline float* GetUV1() { return &m_UV1[0]; }


		inline void SetHandle(unsigned int handle) { m_Handle = handle; }
		inline void SetData(unsigned char* data) { m_Data = data; }
		inline void SetXPos(unsigned int x) { m_XPos = x; }
		inline void SetYPos(unsigned int y) { m_YPos = y; }
		inline void SetWidth(unsigned int width) { m_Width = width; }
		inline void SetHeight(unsigned int height) { m_Height = height; }
		inline void SetUV0(float x, float y) { m_UV0[0] = x; m_UV0[1] = y; }
		inline void SetUV1(float x, float y) { m_UV1[0] = x; m_UV1[1] = y; }
		
	private:
		unsigned int m_XPos;
		unsigned int m_YPos;
		unsigned int m_Width;
		unsigned int m_Height;
		float m_UV0[2];
		float m_UV1[2];

		unsigned int m_Handle;
		unsigned char* m_Data;
	};

	class IconLibrary
	{
	public:
		IconLibrary();
		~IconLibrary();

		bool AddIconsFromFile(std::string& filepath);

		inline void AddIcon(std::string& name, Icon& icon) { m_Icons[name] = icon; }
		inline Icon& GetIcon(std::string& name) { return m_Icons[name]; }
		
		Icon& GetIconFromFileExtension(const std::filesystem::path& path);

	private:
		std::map<std::string, Icon> m_Icons;
	};
}