/* core osiris includes */
#include <core/export.h>
#include <core/Timestep.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/renderer/Renderer.h>
#include <core/renderer/Shader.h>
#include <core/renderer/Buffer.h>
#include <core/renderer/VertexArray.h>
#include <core/scene/GameObject.h>


/* local includes */
#include "datamodels/OrthographicCameraController.h"
#include "support/IconLibrary.h"

namespace Osiris::Editor
{
	class GridGizmo
	{
	public:
		GridGizmo(std::shared_ptr<OrthographicCameraController> cameraController);
		~GridGizmo();

		void Render(Timestep ts, Renderer& renderer);

	private:
		std::shared_ptr<OrthographicCameraController> _CameraController;
		std::shared_ptr<GameObject>		_GameObject;

		std::shared_ptr<Shader>			_Shader;
		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer; 
		std::shared_ptr<IndexBuffer>	_IndexBuffer;
	};
}