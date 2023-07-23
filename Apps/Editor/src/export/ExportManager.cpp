#include <wyrdpch.h>
#include <core/Log.h>
#include <core/Application.h>
#include <core/Resources.h>
#include <core/behaviour/Behaviour.h>
#include <core/renderer/Shader.h>
#include <serial/ComponentSerialiserFactory.h>
#include <serial/ResourceSerialisers.h>

/* export headers */
#include "loaders/SceneLoader.h"

#include "ExportManager.h"
#include "services/ServiceManager.h"
#include "services/WorkspaceService.h"
#include "support/Utils.h"


namespace Wyrd::Editor
{
	bool ExportManager::Export()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>();
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();
		std::shared_ptr<Scene> scene = workspaceService->GetLoadedScene();

		std::ofstream commonBundle;

		// Save current scene
		workspaceService->SaveScene();
		
		std::filesystem::path bundlesDir = workspaceService->GetBuildsDirectory() / "bundles";
		if (Utils::CreateFolder(bundlesDir))
		{
			return false;
		}

		GenerateCoreFile();
		GenerateGameFile();
		GenerateCommonBundleFile();

		for (auto& exportableScene : project->GetExportSettings().exportableScenes)
		{
			GenerateSceneBundleFile(exportableScene.first, bundlesDir);
		}

		WYRD_CORE_TRACE("Exported Game Files");
		return true;
	}

	void ExportManager::GenerateCoreFile()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>();
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();

		std::ofstream core;
		core.open((workspaceService->GetBuildsDirectory() / "Core.data").string(), std::ios::out | std::ios::binary);

		// Add core configuration settings
		core.write((char*)&project->GetExportSettings().width, sizeof(uint32_t));
		core.write((char*)&project->GetExportSettings().height, sizeof(uint32_t));

		core.close();
	}

	void ExportManager::GenerateGameFile()
	{
		auto workspaceService = ServiceManager::Get<WorkspaceService>();
		std::shared_ptr<Project> project = workspaceService->GetCurrentProject();

		std::ofstream game;
		game.open((workspaceService->GetBuildsDirectory() / "Game.data").string(), std::ios::out | std::ios::binary);

		/* Write the scene map */
		size_t sceneCount = project->GetExportSettings().exportableScenes.size();
		game.write((const char*)&sceneCount, sizeof(size_t));

		for (auto& scene : project->GetExportSettings().exportableScenes)
		{
			char sceneName[128];
			strcpy(sceneName, scene.second.c_str());
			game.write((char*)&sceneName[0], sizeof(char) * 128);

			char sceneUID[64];
			strcpy(sceneUID, scene.first.str().c_str());
			game.write((char*)&sceneUID[0], sizeof(char) * 64);
		}

		// Add game settings
		char initialSceneUID[64];
		strcpy(initialSceneUID, project->GetExportSettings().initialScene.str().c_str());

		game.write(&initialSceneUID[0], sizeof(char) * 64);

		game.close();
	}

	void ExportManager::GenerateCommonBundleFile()
	{
		/* retrieve services */
		auto resourceService = ServiceManager::Get<ResourceService>();
		auto workspaceService = ServiceManager::Get<WorkspaceService>();

		/* we need to get a map of just the texture resources */
		//std::map<UID, TextureRes*> textures;
		//for (auto& res : resourceService->GetResources())
		//{
		//	if (res.second->GetType() == ResourceType::TEXTURE)
		//	{
		//		textures.insert({ res.first, std::dynamic_pointer_cast<TextureRes>(res.second).get() });
		//	}
		//}

		/* we need to get a map of just the material resources */
		std::map<UID, MaterialRes*> materials;
		for (auto& res : resourceService->GetResources())
		{
			if (res.second->GetType() == ResourceType::MATERIAL)
			{
				materials.insert({ res.first, std::dynamic_pointer_cast<MaterialRes>(res.second).get() });
			}
		}

		/* we need to get a map of just the model resources */
		std::map<UID, ModelRes*> models;
		for (auto& res : resourceService->GetResources())
		{
			if (res.second->GetType() == ResourceType::MODEL)
			{
				models.insert({ res.first, std::dynamic_pointer_cast<ModelRes>(res.second).get() });
			}
		}

		/* we need to get a map of just the shader resources */
		std::map<UID, ShaderRes*> shaders;
		for (auto& res : resourceService->GetResources())
		{
			if (res.second->GetType() == ResourceType::SHADER)
			{
				shaders.insert({ res.first, std::dynamic_pointer_cast<ShaderRes>(res.second).get() });
			}
		}
		
		
		/* open the common bundle stream */
		std::ofstream commonBundle;
		commonBundle.open((workspaceService->GetBuildsDirectory() / "common.bundle").string(), std::ios::out | std::ios::binary);

		WYRD_CORE_TRACE("Export Core Resources:");

		/* add the texture resources */
		size_t textureCount = Application::Get().GetResources().Textures.size();
		WYRD_CORE_TRACE("\tTextures: {0}", textureCount);
		commonBundle.write((char*)&textureCount, sizeof(size_t));
		for (auto& t : Application::Get().GetResources().Textures)
		{
			SerialiseTexture(commonBundle, t.second);
			WYRD_CORE_TRACE("\t\t{0} -> {1}", t.first.str(), t.second->GetName());
		}

		/* add the model resources */
		size_t modelCount = Application::Get().GetResources().Meshs.size();
		WYRD_CORE_TRACE("\tModels: {0}", modelCount);
		commonBundle.write((char*)&modelCount, sizeof(size_t));
		for (auto& m : Application::Get().GetResources().Meshs)
		{
			SerialiseModel(commonBundle, m.second);
			WYRD_CORE_TRACE("\t\t{0} -> {1}", m.first.str(), m.second->GetName());
		}

		/* add the shader resources */
		size_t shaderCount = Application::Get().GetResources().Shaders.size();
		WYRD_CORE_TRACE("\tShader: {0}", shaderCount);
		commonBundle.write((char*)&shaderCount, sizeof(size_t));
		for (auto& s : Application::Get().GetResources().Shaders)
		{
			SerialiseShader(commonBundle, s.second);
			WYRD_CORE_TRACE("\t\t{0} -> {1}", s.first.c_str(), s.second->GetName());
		}

		/* add the material resources */
		size_t materialCount = Application::Get().GetResources().Materials.size();
		WYRD_CORE_TRACE("\tMaterials: {0}", materialCount);
		commonBundle.write((char*)&materialCount, sizeof(size_t));
		for (auto& m : Application::Get().GetResources().Materials)
		{
			SerialiseMaterial(commonBundle, m.second);
			WYRD_CORE_TRACE("\t\t{0} -> {1}", m.first.str(), m.second->GetName());
		}

		///* add the shader resources */
		//size_t shaderCount = Application::Get().GetResources().Shaders.size();
		//commonBundle.write((char*)&shaderCount, sizeof(size_t));
		//for (auto& s : Application::Get().GetResources().Shaders)
		//{
		//	char shaderName[32];
		//	strcpy(shaderName, s.first.c_str());
		//
		//	std::string vertSrc = s.second->GetSource(ShaderStage::Vertex); 
		//	std::string fragSrc = s.second->GetSource(ShaderStage::Fragment);
		//
		//	size_t vertSrcSize = vertSrc.size();
		//	size_t fragSrcSize = fragSrc.size();
		//
		//	commonBundle.write((char*)shaderName, sizeof(char) * 32);
		//	commonBundle.write((char*)&vertSrcSize, sizeof(size_t));
		//	commonBundle.write((char*)vertSrc.c_str(), sizeof(char) * vertSrcSize);
		//	commonBundle.write((char*)&fragSrcSize, sizeof(size_t));
		//	commonBundle.write((char*)fragSrc.c_str(), sizeof(char) * fragSrcSize);
		//}
		//
		///* add the mesh resources */
		//size_t meshCount = models.size();
		//commonBundle.write((char*)&meshCount, sizeof(size_t));
		//for (auto& m : models)
		//{
		//	char modelUID[64];
		//	strcpy(modelUID, m.first.str().c_str());
		//	size_t vertexCount = m.second->GetMesh()->Vertices.size();
		//
		//	size_t nameSize = m.second->GetName().size();
		//
		//	commonBundle.write(&modelUID[0], sizeof(char) * 64); 
		//	commonBundle.write((char*)&nameSize, sizeof(size_t));
		//	commonBundle.write((char*)m.second->GetName().c_str(), sizeof(char) * nameSize);
		//	commonBundle.write((char*)&vertexCount, sizeof(size_t));
		//	commonBundle.write((char*)&m.second->GetMesh()->Vertices[0], sizeof(Vertex3D) * vertexCount);
		//
		//	WYRD_CORE_INFO("Exporting Mesh: {0} -> {1}", m.first.str().c_str(), m.second->GetName());
		//}

		///* add the texture resources */
		//size_t textureCount = textures.size();
		//commonBundle.write((char*)&textureCount, sizeof(size_t));
		//for (auto& t : textures)
		//{
		//	commonBundle << *t.second->GetTexture();
		//
		//
		//	//char textureUID[64];
		//	//strcpy(textureUID, t.first.str().c_str());
		//	//
		//	//uint32_t width = (uint32_t)t.second->GetWidth();
		//	//uint32_t height = (uint32_t)t.second->GetHeight();
		//	//unsigned char* data = t.second->GetData();
		//	//
		//	//commonBundle.write(&textureUID[0], sizeof(char) * 64);
		//	//commonBundle.write((char*)&width, sizeof(uint32_t));
		//	//commonBundle.write((char*)&height, sizeof(uint32_t));
		//	//commonBundle.write((char*)data, sizeof(unsigned char) * (width * height * 4));
		//}

		/* add the font resources */
		//size_t fontCount = Application::Get().GetResources().FontTypes.size();
		//commonBundle.write((char*)&fontCount, sizeof(size_t));
		//for (auto& f : Application::Get().GetResources().FontTypes)
		//{
		//	char fontName[64];
		//	strcpy(fontName, f.first.c_str());
		//
		//	uint32_t width = (uint32_t)f.second->Texture->GetWidth();
		//	uint32_t height = (uint32_t)f.second->Texture->GetHeight();
		//	uint32_t channels = (uint32_t)f.second->Texture->GetChannels();
		//	unsigned char* data = f.second->Texture->GetData();
		//
		//	commonBundle.write((char*)fontName, sizeof(char) * 64);
		//	commonBundle.write((char*)&width, sizeof(uint32_t));
		//	commonBundle.write((char*)&height, sizeof(uint32_t));
		//	commonBundle.write((char*)&channels, sizeof(uint32_t));
		//	commonBundle.write((char*)data, sizeof(unsigned char) * (width * height * channels));
		//
		//	size_t characterCount = f.second->GetCharacters().size();
		//	commonBundle.write((char*)&characterCount, sizeof(size_t));
		//	for (auto& c : f.second->GetCharacters())
		//	{
		//		commonBundle.write((char*)&c.second.Advance, sizeof(unsigned int));
		//		commonBundle.write((char*)&c.second.Bearing, sizeof(glm::ivec2));
		//		commonBundle.write((char*)&c.second.Size, sizeof(glm::ivec2));
		//		commonBundle.write((char*)&c.second.uv1, sizeof(glm::vec2));
		//		commonBundle.write((char*)&c.second.uv2, sizeof(glm::vec2));
		//	}
		//}
		//
		
		/* build a list of the script files */
		std::map<UID, std::filesystem::path> scriptFiles;
		for (auto& [key, value] : resourceService->GetResources())
		{
			auto downcastedPtr = std::dynamic_pointer_cast<ScriptRes>(value);
			if (downcastedPtr)
			{
				scriptFiles.insert({ key , value->GetPath() });
			}
		}
		
		/* write the script file map */
		size_t fileCount = scriptFiles.size();
		commonBundle.write((char*)&fileCount, sizeof(size_t));
		for (auto& file : scriptFiles)
		{
			char fileNamebuffer[64];
			strcpy(fileNamebuffer, file.second.stem().string().c_str());
			commonBundle.write((char*)&fileNamebuffer, sizeof(char) * 64);
		
			char uuidBuffer[64];
			strcpy(uuidBuffer, file.first.str().c_str());
			commonBundle.write((char*)&uuidBuffer, sizeof(char) * 64);
		}
		
		/* TODO: Remove hardcoded path */
		/* load the client and core library and add to the file stream */
		std::filesystem::path coreLibraryLocation = "C:/Projects/Wyrd/WyrdEngine/lib/Debug/WyrdAPI/WyrdAPI.dll";
		std::filesystem::path clientLibraryLocation = workspaceService->GetLoadedProjectPath().parent_path() / (workspaceService->GetCurrentProject()->name + ".dll");
		
		std::ifstream coreLibFileStream(coreLibraryLocation.c_str(), std::ios::binary);
		std::vector<char> coreLibData((std::istreambuf_iterator<char>(coreLibFileStream)), (std::istreambuf_iterator<char>()));
		coreLibFileStream.close();
		
		std::ifstream clientLibFileStream(clientLibraryLocation.c_str(), std::ios::binary);
		std::vector<char> clientLibData((std::istreambuf_iterator<char>(clientLibFileStream)), (std::istreambuf_iterator<char>()));
		clientLibFileStream.close();
		
		size_t coreLibFileSize = coreLibData.size(); 
		size_t clientLibFileSize = clientLibData.size();
		
		commonBundle.write((char*)&coreLibFileSize, sizeof(size_t));
		commonBundle.write(&coreLibData[0], sizeof(char) * coreLibFileSize);
		
		commonBundle.write((char*)&clientLibFileSize, sizeof(size_t));
		commonBundle.write(&clientLibData[0], sizeof(char) * clientLibFileSize);

		commonBundle.close();
	}

	void ExportManager::GenerateSceneBundleFile(const Wyrd::UID& sceneUID, const std::filesystem::path& dir)
	{
		auto resourceService = ServiceManager::Get<ResourceService>();
		auto sceneRes = resourceService->GetResourceByID<SceneRes>(sceneUID);

		/* open the scene file stream */
		std::ofstream sceneBundle;
		sceneBundle.open(dir / (sceneUID.str() + ".bundle"), std::ios::out | std::ios::binary);

		/* load the scene */
		Scene scene;
		scene.Initialise();
		SceneLoader::Load(sceneRes->GetPath(), scene, false);

		/* write the camera entity */
		Entity cameraEntity = scene.GetPrimaryCameraEntity();
		sceneBundle.write((char*)&cameraEntity, sizeof(Entity));

		/* write the entity list */
		size_t entityCount = scene.entities.size();
		sceneBundle.write((char*)&entityCount, sizeof(size_t));
		for (auto& e : scene.entities)
		{
			uint64_t mask = e.mask.to_ullong();

			sceneBundle.write((char*)&e, sizeof(Entity));
			sceneBundle.write((char*)&mask, sizeof(uint64_t));
		}

		/* write the component pool arrays */
		size_t componentPoolCount = 0;
		for (auto& cp : scene.componentPools)
		{
			if (cp->serialise)
				componentPoolCount++;
		}

		// TODO: Need to convert to a serialisation factory calls
		sceneBundle.write((char*)&componentPoolCount, sizeof(size_t));
		for (size_t poolIndex = 0; poolIndex < componentPoolCount; poolIndex++)
		{
			const ComponentPool& cp = *scene.componentPools[poolIndex];
			if (cp.serialise)
			{
				char poolName[64];
				size_t poolElementSize;
				size_t poolElementCount;

				strcpy(&poolName[0], cp.name.c_str());
				poolElementSize = cp.elementSize;
				poolElementCount = cp.count;

				sceneBundle.write((char*)&poolName, sizeof(char) * 64);
				sceneBundle.write((char*)&poolElementSize, sizeof(size_t));
				sceneBundle.write((char*)&poolElementCount, sizeof(size_t));

				for (size_t elementIndex = 0; elementIndex < poolElementCount; elementIndex++)
				{
					ComponentSerialiserFactory::Serialise(sceneBundle, poolName, &(scene.componentPools[poolIndex]->data[elementIndex * poolElementSize]));
					scene.componentPools[poolIndex]->count++;
				}
				//sceneBundle.write(cp.data, poolElementSize * poolElementCount);
			}
		}

		sceneBundle.close();
	}
}