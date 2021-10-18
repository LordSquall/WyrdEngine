#include "core/export.h"

namespace Wyrd::Editor
{
	class ExportManager
	{
	public:
		ExportManager() {}
		~ExportManager() = default;

		static bool Export();

		static void GenerateCoreFile();
		static void GenerateGameFile();
		static void GenerateCommonBundleFile();
		static void GenerateSceneBundleFile(const Wyrd::UID& sceneUID, const std::filesystem::path& dir);
	};
}