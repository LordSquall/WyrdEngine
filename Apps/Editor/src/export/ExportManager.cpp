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

		/* Create initial resource include lists */
		std::vector<TextureResRef> textureResources;
		std::vector<ModelResRef> modelResources;
		std::vector<ShaderResRef> shaderResources;
		std::vector<MaterialResRef> materialResources;


		for (auto [k, v] : resourceService->GetResources())
		{
			if (!v->IsEditorOnly())
			{
				switch (v->GetType())
				{
				case TEXTURE:
					textureResources.push_back(std::dynamic_pointer_cast<TextureRes>(v));
					break;
				case MODEL:
					modelResources.push_back(std::dynamic_pointer_cast<ModelRes>(v));
					break;
				case SHADER:
					shaderResources.push_back(std::dynamic_pointer_cast<ShaderRes>(v));
					break;
				case MATERIAL:
					materialResources.push_back(std::dynamic_pointer_cast<MaterialRes>(v));
					break;
				default:
					break;
				}
			}
		}

		/* add the texture resources */
		size_t textureCount = textureResources.size();
		WYRD_CORE_TRACE("\tTextures: {0}", textureCount);
		commonBundle.write((char*)&textureCount, sizeof(size_t));
		for (TextureResRef t : textureResources)
		{
			SerialiseTexture(commonBundle, t->GetTexture());
			WYRD_CORE_TRACE("\t\t{0} -> {1}", t->GetResourceID().str(), t->GetName());
		}

		/* add the model resources */
		size_t modelCount = modelResources.size();
		WYRD_CORE_TRACE("\tModels: {0}", modelCount);
		commonBundle.write((char*)&modelCount, sizeof(size_t));
		for (ModelResRef m : modelResources)
		{
			SerialiseModel(commonBundle, m->GetMesh());
			WYRD_CORE_TRACE("\t\t{0} -> {1}", m->GetResourceID().str(), m->GetName());
		}

		/* add the shader resources */
		size_t shaderCount = shaderResources.size();
		WYRD_CORE_TRACE("\tShader: {0}", shaderCount);
		commonBundle.write((char*)&shaderCount, sizeof(size_t));
		for (ShaderResRef s : shaderResources)
		{
			SerialiseShader(commonBundle, s->GetShader());
			WYRD_CORE_TRACE("\t\t{0} -> {1}", s->GetShader()->GetUID().str(), s->GetName());
		}

		/* add the material resources */
		size_t materialCount = materialResources.size();
		WYRD_CORE_TRACE("\tMaterials: {0}", materialCount);
		commonBundle.write((char*)&materialCount, sizeof(size_t));
		for (MaterialResRef m : materialResources)
		{
			SerialiseMaterial(commonBundle, m->GetMaterial());
			WYRD_CORE_TRACE("\t\t{0} -> {1}", m->GetResourceID().str(), m->GetName());
		}
		
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