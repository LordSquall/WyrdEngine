#pragma once

/* local includes */
#include "support/IconLibrary.h"

/* core includes */
#include "core/maths/Rect.h"

/* external includes */
#include <glm/glm.hpp>

namespace Osiris::Editor
{
	/* forward declarations */
	class EditorLayer;

	/**
	 * @brief Configuration structure of the editor view. 
	 * This structure is used to control the common properties of a editor view.
	*/
	struct EditorViewConfig
	{
		bool allowMenuBar = false;
		float windowPaddingX = 8.0f;
		float windowPaddingY = 8.0f;
	};

	/**
	 * @brief Base class for the editor views
	*/
	class EditorViewBase
	{
	public:
		EditorViewBase(const std::string& name, EditorLayer* editorLayer) { _Name = name; _EditorLayer = editorLayer; }
		virtual ~EditorViewBase() { }

		/**
		 * @brief Called on Editor initialisation
		*/
		virtual void OnInitialise() {};

		/**
		 * @brief Called once per frame
		 * @param ts 
		*/
		virtual void OnUpdate(Timestep ts) {};

		/**
		 * @brief Called once per Render operation
		 * @param ts 
		 * @param renderer 
		*/
		virtual void OnRender(Timestep ts, Renderer& renderer) {};

		/**
		 * @brief Called when an event is targeted at this view
		 * @param event 
		*/
		virtual void OnEvent(Event& event) {};

		/**
		 * @brief Called once per Editor Render operation
		*/
		virtual void OnEditorRender() {};

		/**
		 * @brief Called on a resize event
		 * Note: Possible dupication with the event system. Consider refactoring
		*/
		virtual void OnResize() {};

		/**
		 * @brief Called prior to the On Editor render function.
		 * This function is responsible for settings to editor render state before rendering the view
		*/
		void OnPreEditorRender();

		/**
		 * @brief Called after the On Editor render function.
		 * This function is responible for clearing any state based editor render settings
		*/
		void OnPostEditorRender();

		/**
		 * @brief Get the rendering boundary of the view
		 * @return 
		*/
		inline Osiris::Rect GetBoundary() const { return _Boundary; }

		/**
		 * @brief Tranform a point to view space
		 * @return
		*/
		glm::vec2 TransformToViewSpace(glm::vec2 point);

		/**
		 * @brief Get the Name
		 * @return 
		*/
		inline const std::string& GetName() const { return _Name; }

		/**
		 * @brief Close the view 
		*/
		inline void Close() { _ShowFlag = false; }

		/**
		 * @brief Get the Show Flag reference
		 * @return 
		*/
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