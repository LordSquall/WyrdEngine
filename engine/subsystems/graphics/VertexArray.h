#ifndef _VERTEXARRAY_H_
#define _VERTEXARRAY_H_

#include "export.h"

#include <vector>

#include "subsystems\graphics\VertexBuffer.h"
#include "subsystems\common\Structures.h"

using namespace std;

namespace OrisisEngine
{

	class IRenderer;

	class VertexArray {
	public:
		VertexArray();
		VertexArray(const VertexArray &obj);
		~VertexArray();

		void AddVertexBuffer(VertexBuffer* vertexBuffer);

		void Generate(IRenderer* renderer);

	public:
		vector<VertexBuffer*> _vertexBuffers;

		unsigned int Handle;
	};
}

#endif