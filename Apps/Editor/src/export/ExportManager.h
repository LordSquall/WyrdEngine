#include "core/export.h"

namespace Wyrd::Editor
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
		static void GenerateSceneBundleFile(const Wyrd::UID& sceneUID, const std::string& dir);
	};
}