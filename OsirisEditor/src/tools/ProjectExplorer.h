#include "core/export.h"
#include "core/Layer.h"

#include "EditorPlugin.h"
#include "IconLibrary.h"

namespace Osiris::Editor
{
	struct ProjectExplorer_entry
	{
		static unsigned int s_NextId;

		ProjectExplorer_entry(std::string& rootPath) {
			path = rootPath; 
			uid = s_NextId;
			s_NextId++;
		}

		unsigned int uid;

		std::string path;

		std::map<std::string, ProjectExplorer_entry> children;
	};


	class ProjectExplorer_data
	{
	public:
		ProjectExplorer_data() {}
		virtual ~ProjectExplorer_data() {}

		void Update(ProjectExplorer_entry& root);

		std::map<std::string, ProjectExplorer_entry> filesystem;
	};

	class OSIRIS_API ProjectExplorer : public EditorPlugin
	{
	public:
		ProjectExplorer();
		~ProjectExplorer();
		
		void OnInitialise() override;
		void OnEditorRender() override;

	private:
		void PopulateNavigationPanel();
		void PopulateContentPanel();
		void PopulateSubFileTree(ProjectExplorer_entry& entry);

		void CreateFolder(std::string& path);

	private:
		std::string m_CurrentDir;
		ProjectExplorer_data m_FSData;
	};
}