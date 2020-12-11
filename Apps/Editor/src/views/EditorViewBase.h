#pragma once

/* local includes */
#include "support/IconLibrary.h"

/* core includes */
#include "core/maths/Rect.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris::Editor
{
	class EditorLayer;

	struct EditorViewConfig
	{
		bool allowMenuBar = false;
		float windowPaddingX = 8.0f;
		float windowPaddingY = 8.0f;
	};

	class EditorViewBase
	{
	public:
		EditorViewBase(const std::string& name, EditorLayer* editorLayer) { _Name = name; _EditorLayer = editorLayer; }
		virtual ~EditorViewBase() { }

		virtual void OnInitialise() {};
		virtual void OnUpdate(Timestep ts) {};
		virtual void OnRender(Timestep ts, Renderer& renderer) {};
		virtual void OnEvent(Event& event) {};
		virtual void OnEditorRender() {};

		virtual void OnResize() {};

		void OnPreEditorRender();
		void OnPostEditorRender();

		inline Osiris::Rect GetBoundary() const { return _Boundary; }
		inline const std::string& GetName() const { return _Name; }

		inline void Close() { _ShowFlag = false; }
		inline bool* GetShowFlagRef() { return &_ShowFlag; }

	public:
		EditorViewConfig	config;

	protected:
		EditorLayer*		_EditorLayer;
		std::string			_Name;
		bool				_ShowFlag = true;
		Osiris::Rect		_Boundary;
		Osiris::Rect		_PrevBoundary;
	};
}