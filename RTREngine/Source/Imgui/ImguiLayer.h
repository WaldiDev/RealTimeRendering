#pragma once
#include "Core/Layer.h"

namespace rtr
{
	class ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;

		void Begin();
		void End();
	private:

	};
}
