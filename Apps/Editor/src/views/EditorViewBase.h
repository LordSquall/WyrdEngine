#pragma once

#include "support/IconLibrary.h"

#include <glm/glm.hpp>

namespace Osiris::Editor
{
	struct EditorViewConfig
	{
		bool allowMenuBar = false;
		float windowPaddingX = 8.0f;
		float windowPaddingY = 8.0f;
	};

	class EditorViewBase
	{
	public:
		EditorViewBase(const std::string& name) { _Name = name; }
		virtual ~EditorViewBase() { }

		virtual void OnInitialise() {};
		virtual void OnRender(Timestep ts, Renderer& renderer) {};
		virtual void OnEvent(Event& event) {};
		virtual void OnEditorRender() {};

		virtual void OnResize() {};

		void OnPreEditorRender();
		void OnPostEditorRender();

		inline const std::string& GetName() const { return _Name; }
		inline glm::vec4 GetBoundary() const { return _Boundary; }

		inline void Close() { _ShowFlag = false; }
		inline bool* GetShowFlagRef() { return &_ShowFlag; }

	public:
		EditorViewConfig	config;

	protected:
		std::string			_Name;
		bool				_ShowFlag = true;
		glm::vec4			_Boundary;
		glm::vec4			_PrevBoundary;
	};
}