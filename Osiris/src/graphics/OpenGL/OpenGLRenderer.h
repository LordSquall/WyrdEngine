#pragma once

#include "core/Renderer.h"

namespace Osiris {

	class OpenGLRenderer : public Renderer
	{
	public:
		OpenGLRenderer();

		virtual ~OpenGLRenderer();


		unsigned int LoadShader(std::shared_ptr<Shader> shader) override;
		unsigned int LoadMesh(std::unique_ptr<Mesh>& mesh) override;
		unsigned int LoadTexture(std::shared_ptr<Texture> texture) override;

		unsigned int LoadRawTexture(unsigned char* data, int width, int height, int channels);

		unsigned int RenderMesh(std::unique_ptr<Mesh>& mesh) override;

	private:
		unsigned int CompileShader(unsigned int type, const std::string& source);
	};
}