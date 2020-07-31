#include <string>
#include <LilEngie.h>
#include "IEditorWindow.h"

namespace LilEddie
{
	class ToolbarWindow : public IEditorWindow
	{
		WINDOW_TITLE("Toolbar")

	private:
		float groupSize = 0;

	public:
		ToolbarWindow(Game* game)
			: IEditorWindow(game) { }

		void Init() override;
		void OnDraw() override;
	};
}