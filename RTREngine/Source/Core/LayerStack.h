#pragma once

#include <vector>

namespace rtr
{
	class Layer;
	using LayerStackIterator = std::vector<Layer*>::iterator;

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);

		LayerStackIterator begin() { return mLayers.begin(); }
		LayerStackIterator end() { return mLayers.end(); }

	private:
		std::vector<Layer*> mLayers;
		unsigned int mLayerInsertIndex = 0;
	};
}
