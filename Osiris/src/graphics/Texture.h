#pragma once

#include "core/export.h"

namespace Osiris {

	class OSIRIS_API Texture
	{
	public:
		/* Constructions/Destruction */
		Texture();
		virtual ~Texture();

		/* Getters */
		inline const std::string& GetName() { return m_name; }
		inline const std::string& GetSourceFileName() { return m_srcFilename; }
		inline const int GetWidth() { return m_width; }
		inline const int GetHeight() { return m_width; }
		inline const unsigned char* GetData() { return m_data; }
		inline const unsigned int GetHandle() { return m_handle; }
		inline const unsigned int GetChannels() { return m_channels; }

		/* Setters */
		inline void SetName(std::string& name) { m_name = name; }
		inline void SetSourceFileName(const std::string& srcFilename) { m_srcFilename = srcFilename; }
		inline void SetWidth(int width) { m_width = width; }
		inline void SetHeight(int height) { m_height = height; }
		inline void SetData(unsigned char* data) { m_data = data; }
		inline void SetHandle(unsigned int handle) { m_handle = handle; }
		inline void SetChannels(unsigned int channels) { m_channels = channels; }


		/* Loading functions */
		static std::shared_ptr<Texture> CreateFromFile(const std::string& filePath);
	private:

		std::string		m_name;			// unique name
		std::string		m_srcFilename;	// source file name
		int				m_width;		// width in pixels
		int				m_height;		// height in pixels
		unsigned char*	m_data;			// ptr to raw pixel data
		unsigned int	m_handle;		// renderer handle
		unsigned int	m_channels;		// channels
	};
}