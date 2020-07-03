#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class HistogramEqualizationFilter : public FilterInterface
	{
	public:
		HistogramEqualizationFilter(int grayScale);
		virtual void Apply(Image&) const override;

	private:
		int m_GrayScale;
	};
}