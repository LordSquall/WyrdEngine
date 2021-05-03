#pragma once

#include <wyrdpch.h>
#include <core/UID.h>

namespace Wyrd
{
	class Scene;
};

class ImportManager
{
public:
	static void ImportCore(const std::string& root);
	static void ImportGame(const std::string& root);
	static void ImportCommonBundle(const std::string& root);
	static std::unique_ptr<Wyrd::Scene> ImportScene(const std::string& root, const Wyrd::UID& sceneUID);
};