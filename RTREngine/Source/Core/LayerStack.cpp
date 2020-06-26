#include "LayerStack.h"
#include "Core/Logging/Log.h"
#include "Layer.h"

namespace rtr
{
	LayerStack::LayerStack()
		: mLayers()
		, mLayerInsertIndex(0u)
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers)
		{
			layer->OnDetach();
			delete layer;
		}

		mLayers.clear();
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		RTR_ENGINE_TRACE("Push layer {0}", layer->GetName());
		mLayers.emplace(mLayers.begin() + mLayerInsertIndex, layer);
		mLayerInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		RTR_ENGINE_TRACE("Push overlay {0}", overlay->GetName());
		mLayers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		RTR_ENGINE_TRACE("Pop layer {0}", layer->GetName());

		LayerStackIterator layerIter = std::find(mLayers.begin(), mLayers.begin() + mLayerInsertIndex, layer);
		if (layerIter != (mLayers.begin() + mLayerInsertIndex))
		{
			layer->OnDetach();
			mLayers.erase(layerIter);
			mLayerInsertIndex--;
		}
		else
		{
			RTR_ENGINE_WARN("Pop layer {0} failed", layer->GetName());
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		RTR_ENGINE_TRACE("Pop overlay {0}", overlay->GetName());

		LayerStackIterator layerIter = std::find(mLayers.begin() + mLayerInsertIndex, mLayers.end(), overlay);
		if (layerIter != mLayers.end())
		{
			overlay->OnDetach();
			mLayers.erase(layerIter);
		}
		else
		{
			RTR_ENGINE_WARN("Pop overlay {0} failed", overlay->GetName());
		}
	}
}
