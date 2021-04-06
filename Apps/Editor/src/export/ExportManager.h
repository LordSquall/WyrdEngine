#include "core/export.h"

namespace Osiris::Editor
{
	class ExportManager
	{
	public:
		ExportManager() {}
		~ExportManager() = default;

		static void Export();

		static void GenerateCoreFile();
		static void GenerateGameFile();
		static void GenerateCommonBundleFile();
		static void GenerateSceneBundleFile(const Osiris::UID& sceneUID, const std::string& dir);
	};
}