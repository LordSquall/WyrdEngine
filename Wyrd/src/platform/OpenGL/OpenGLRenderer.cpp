#include "wyrdpch.h"
#include "OpenGLRenderer.h"
#include "core/Log.h"
#include "core/Application.h"

#include "core/renderer/Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

namespace Wyrd
{
	OpenGLRenderer::OpenGLRenderer()
	{
		std::ostringstream stringStream;
		stringStream << glGetString(GL_VENDOR);
		_vendorInfo.vendor = stringStream.str();
		stringStream.str(std::string());

		stringStream << glGetString(GL_VERSION);
		_vendorInfo.version = stringStream.str();
		stringStream.str(std::string());

		stringStream << glGetString(GL_RENDERER);
		_vendorInfo.renderer = stringStream.str();
		stringStream.str(std::string());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);

		_DebugVertexBuffer.reset(VertexBuffer::Create((float*)&_DebugVertices, sizeof(Vertex3D), "debugVector3Dbatch"));

		_DebugVertexArray.reset(VertexArray::Create());
		_DebugVertexArray->SetAttribute(0, 0, 3, sizeof(Vertex3D));
	}

	OpenGLRenderer::~OpenGLRenderer()
	{
	}


	void OpenGLRenderer::SetViewport(float width, float height)
	{
		glViewport(0, 0, width, height);
	}

	void OpenGLRenderer::Clear(float r, float g, float b) const
	{
		glClearColor(r, g, b, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRenderer::DrawElements(RendererDrawType type, uint32_t count) const
	{
		GLuint primitiveType = 0;

		/* map renderer primitive to opengl primitive */
		switch (type)
		{
		case RendererDrawType::Triangles:
			primitiveType = GL_TRIANGLES;
			break;
		case RendererDrawType::TriangleStrip:
			primitiveType = GL_TRIANGLE_STRIP;
			break;
		case RendererDrawType::TriangleFan:
			primitiveType = GL_TRIANGLE_FAN;
			break;
		}

		glDrawElements(primitiveType, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderer::DrawArray(RendererDrawType type, uint32_t offset, uint32_t count) const
	{
		GLuint primitiveType = 0;

		/* map renderer primitive to opengl primitive */
		switch (type)
		{
		case RendererDrawType::Triangles:
			primitiveType = GL_TRIANGLES;
			break;
		case RendererDrawType::TriangleStrip:
			primitiveType = GL_TRIANGLE_STRIP;
			break;
		case RendererDrawType::TriangleFan:
			primitiveType = GL_TRIANGLE_FAN;
			break;
		case RendererDrawType::Quads:
			primitiveType = GL_QUADS;
			break;
		case RendererDrawType::Lines:
			primitiveType = GL_LINES;
			break;
		}

		glDrawArrays(primitiveType, offset, count);
	}


	void OpenGLRenderer::DrawDebugBoundingBox(const BoundingBox& boundingBox, const glm::vec3& position, float thickness, const Color& color, const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view)
	{
		glm::vec3 c = position + boundingBox._position;
		glm::vec3 min = boundingBox._minExtent;
		glm::vec3 max = boundingBox._maxExtent;
		_DebugVertices.push_back({ c.x + min.x, c.y + min.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + max.y, c.z + min.z , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + max.y, c.z + min.z , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + max.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + max.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + min.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + min.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + min.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
																 
		_DebugVertices.push_back({ c.x + min.x, c.y + min.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + max.y, c.z + max.z , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + max.y, c.z + max.z , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + max.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + max.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + min.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + min.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + min.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
														 
		_DebugVertices.push_back({ c.x + min.x, c.y + min.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + min.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + max.y, c.z + min.z , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + min.x, c.y + max.y, c.z + max.z , 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + max.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + max.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + max.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + max.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + min.y, c.z + min.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ c.x + max.x, c.y + min.y, c.z + max.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });

		auto shader = Application::Get().GetResources().Shaders["Debug3D"];

		glLineWidth((GLfloat)thickness);

		shader->Bind();
		shader->SetMatrix("u_model", model);
		shader->SetMatrix("u_view", view);
		shader->SetMatrix("u_projection", projection);

		shader->SetUniformColor("u_BlendColor", color);

		_DebugVertexArray->Bind();
		_DebugVertexBuffer->Bind();


		/* update both the vertex and index buffers */
		_DebugVertexBuffer->Update((float*)&_DebugVertices.at(0), sizeof(Vertex3D) * (uint32_t)_DebugVertices.size(), 0);

		DrawArray(RendererDrawType::Lines, 0, (uint32_t)_DebugVertices.size());

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		_Renderer->EndNamedSection();
#endif
		_DebugVertices.clear();
	}

	void OpenGLRenderer::DrawDebugVector(const Vector3& position, const glm::vec3& direction, float thickness, const Color& color, const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view)
	{
		_DebugVertices.push_back({ position.x, position.y, position.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ position.x + direction.x, position.y + direction.y, position.z + direction.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });

		auto shader = Application::Get().GetResources().Shaders["Debug3D"];

		glLineWidth((GLfloat)thickness);

		shader->Bind();
		shader->SetMatrix("u_model", model);
		shader->SetMatrix("u_view", view);
		shader->SetMatrix("u_projection", projection);

		shader->SetUniformColor("u_BlendColor", color);

		_DebugVertexArray->Bind();
		_DebugVertexBuffer->Bind();


		/* update both the vertex and index buffers */
		_DebugVertexBuffer->Update((float*)&_DebugVertices.at(0), sizeof(Vertex3D) * (uint32_t)_DebugVertices.size(), 0);

		DrawArray(RendererDrawType::Lines, 0, (uint32_t)_DebugVertices.size());

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		_Renderer->EndNamedSection();
#endif
		_DebugVertices.clear();
	}


	void OpenGLRenderer::DrawDebugFrustum(const Vector3& position, const glm::vec3& direction, const Frustum& frustum, float thickness, const Color& color, const glm::mat4& model, const glm::mat4& projection, const glm::mat4& view)
	{
		glm::vec3 c = { position.x , position.y, position.z };

		glm::vec3 ftl = Plane::IntersectionPoint(frustum.topFace, frustum.farFace, frustum.leftFace);
		glm::vec3 ftr = Plane::IntersectionPoint(frustum.topFace, frustum.farFace, frustum.rightFace);
		glm::vec3 fbr = Plane::IntersectionPoint(frustum.bottomFace, frustum.farFace, frustum.rightFace);
		glm::vec3 fbl = Plane::IntersectionPoint(frustum.bottomFace, frustum.farFace, frustum.leftFace);

		glm::vec3 ntl = Plane::IntersectionPoint(frustum.topFace, frustum.nearFace, frustum.leftFace);
		glm::vec3 ntr = Plane::IntersectionPoint(frustum.topFace, frustum.nearFace, frustum.rightFace);
		glm::vec3 nbr = Plane::IntersectionPoint(frustum.bottomFace, frustum.nearFace, frustum.rightFace);
		glm::vec3 nbl = Plane::IntersectionPoint(frustum.bottomFace, frustum.nearFace, frustum.leftFace);


		// far plane
		_DebugVertices.push_back({ ftl.x, ftl.y, ftl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ftr.x, ftr.y, ftr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ftr.x, ftr.y, ftr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ fbr.x, fbr.y, fbr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ fbr.x, fbr.y, fbr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ fbl.x, fbl.y, fbl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ fbl.x, fbl.y, fbl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ftl.x, ftl.y, ftl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });


		//near plane
		_DebugVertices.push_back({ ntl.x, ntl.y, ntl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ntr.x, ntr.y, ntr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ntr.x, ntr.y, ntr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ nbr.x, nbr.y, nbr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ nbr.x, nbr.y, nbr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ nbl.x, nbl.y, nbl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ nbl.x, nbl.y, nbl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ntl.x, ntl.y, ntl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });

		// joining bounds
		_DebugVertices.push_back({ ntl.x, ntl.y, ntl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ftl.x, ftl.y, ftl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ntr.x, ntr.y, ntr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ ftr.x, ftr.y, ftr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ nbl.x, nbl.y, nbl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ fbl.x, fbl.y, fbl.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ nbr.x, nbr.y, nbr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
		_DebugVertices.push_back({ fbr.x, fbr.y, fbr.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });

		auto shader = Application::Get().GetResources().Shaders["Debug3D"];

		glLineWidth((GLfloat)thickness);

		shader->Bind();
		shader->SetMatrix("u_model", model);
		shader->SetMatrix("u_view", view);
		shader->SetMatrix("u_projection", projection);

		shader->SetUniformColor("u_BlendColor", color);

		_DebugVertexArray->Bind();
		_DebugVertexBuffer->Bind();


		/* update both the vertex and index buffers */
		_DebugVertexBuffer->Update((float*)&_DebugVertices.at(0), sizeof(Vertex3D) * (uint32_t)_DebugVertices.size(), 0);

		DrawArray(RendererDrawType::Lines, 0, (uint32_t)_DebugVertices.size());

#ifdef WYRD_INCLUDE_DEBUG_TAGS
		_Renderer->EndNamedSection();
#endif
		_DebugVertices.clear();
	}


#ifdef WYRD_INCLUDE_DEBUG_TAGS

	void OpenGLRenderer::StartNamedSection(const char* name) const
	{
		glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name);
	}

	void OpenGLRenderer::EndNamedSection() const
	{
		glPopDebugGroup();
	}

#endif
}
