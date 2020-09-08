#pragma once

#include "FilterInterface.h"
#include <functional>

namespace Tecodity {
	class ConvertGrayToRGB : public FilterInterface
	{
	public:
		ConvertGrayToRGB(std::function<RGBColor(int)> transformation)
			: m_Transformation(transformation)
		{};
		virtual void Apply(Image&) const override;

	protected:
		std::function<RGBColor(int)> m_Transformation;
	};
}
