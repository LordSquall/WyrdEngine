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
	static bool ImportCore(const std::string& root);
	static bool ImportGame(const std::string& root);
	static bool ImportCommonBundle(const std::string& root);
	static std::unique_ptr<Wyrd::Scene> ImportScene(const std::string& root, const Wyrd::UID& sceneUID);
};