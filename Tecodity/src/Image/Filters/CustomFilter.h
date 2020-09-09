#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class CustomFilter : public FilterInterface
	{
	protected:
		std::function<void(Image&)> m_Filter;
	public:
		CustomFilter(std::function<void(Image&)> filter)
			: m_Filter(filter) 
		{}
		virtual void Apply(Image& image) const override
		{
			m_Filter(image);
		};
	};
}
