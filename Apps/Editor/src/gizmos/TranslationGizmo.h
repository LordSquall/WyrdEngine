#include "core/export.h"

#include "core/Timestep.h"
#include "core/renderer/Renderer.h"
#include "core/renderer/Shader.h"
#include "core/renderer/Buffer.h"
#include "core/renderer/VertexArray.h"

#include "datamodels/GameObject.h"
#include "datamodels/OrthographicCameraController.h"

#include "support/IconLibrary.h"

namespace Osiris::Editor
{
	class TranslationGizmo
	{
	public:
		TranslationGizmo(std::shared_ptr<Shader> shader, std::shared_ptr<OrthographicCameraController> cameraController);
		~TranslationGizmo();

		void SetGameObject(std::shared_ptr<GameObject> gameObject);

		void OnDrag(glm::vec2 delta);

		void Render(Timestep ts, Renderer& renderer);

	private:
		std::shared_ptr<OrthographicCameraController> _CameraController;
		std::shared_ptr<GameObject>		_GameObject;

		std::shared_ptr<Shader>			_Shader;
		std::shared_ptr<VertexArray>	_VertexArray;
		std::shared_ptr<VertexBuffer>	_VertexBuffer; 
		std::shared_ptr<IndexBuffer>	_IndexBuffer;

		std::shared_ptr<Icon> _Icon;
	};
}