#pragma once

#include <string>

namespace rtr
{
	class Layer
	{
	public:
		explicit Layer(const std::string& name)
			: mName(name) {}

		virtual ~Layer() {}

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}

		const std::string& GetName() const { return mName; }

	protected:
		std::string mName;

	};
}
