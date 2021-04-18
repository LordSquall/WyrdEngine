#pragma once

#include <osrpch.h>
#include <core/UID.h>

namespace Osiris
{
	class Scene;
};

class ImportManager
{
public:
	static void ImportCore(const std::string& root);
	static void ImportGame(const std::string& root);
	static void ImportCommonBundle(const std::string& root);
	static std::unique_ptr<Osiris::Scene> ImportScene(const std::string& root, const Osiris::UID& sceneUID);
};