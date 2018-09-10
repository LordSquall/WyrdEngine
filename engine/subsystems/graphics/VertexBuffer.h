#ifndef _VERTEXBUFFER_H_
#define _VERTEXBUFFER_H_

#include "export.h"

#include <vector>

#include "subsystems\common\Structures.h"

using namespace std;

namespace OrisisEngine
{
	class IRenderer;

	class VertexBuffer {
	public:
		VertexBuffer();
		VertexBuffer(const VertexBuffer &obj);
		~VertexBuffer();

		void AddVertex(Vertex vertex);

		void Generate(IRenderer* renderer);

		unsigned int GetCount();
		vector<Vertex> VertexBuffer::GetArray();

	public:
		unsigned int Handle;

	private:
		vector<Vertex> _vertices;
	};
}

#endif