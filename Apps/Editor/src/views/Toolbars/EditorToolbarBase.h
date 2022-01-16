#pragma once

namespace Wyrd::Editor
{
	class EditorToolbarBase
	{
	public:
		EditorToolbarBase() {};
		virtual ~EditorToolbarBase() { }

		virtual void OnToolbarRender() = 0;
	};
}