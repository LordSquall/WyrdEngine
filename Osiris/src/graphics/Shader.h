#pragma once

#include "core/export.h"

namespace Osiris {

	class OSIRIS_API Shader
	{
	public:
		/* Constructions/Destruction */
		Shader();
		virtual ~Shader();

		/* Getters */
		inline const std::string& GetName() { return m_name; }
		inline const std::string& GetVertexSource() { return m_vertexSrc; }
		inline const std::string& GetFragmentSource() { return m_fragmentSrc; }

		/* Setters */
		inline void SetName(std::string& name) { m_name = name; }
		inline void SetVertexSource(std::string& src) { m_vertexSrc = src; }
		inline void SetFragmentSource(std::string& src) { m_fragmentSrc = src; }


		/* Loading functions */
		static std::shared_ptr<Shader> CreateFromFiles(const std::string& vertexSrcFile, const std::string& fragmentSrcFile);
	private:

		std::string		m_name;			// unique name
		std::string		m_vertexSrc;
		std::string		m_fragmentSrc;
	};
}