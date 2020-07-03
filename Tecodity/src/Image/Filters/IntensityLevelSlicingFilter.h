#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class IntensityLevelSlicingFilter : public FilterInterface
	{
	public:
		IntensityLevelSlicingFilter(int RangeA, int RangeB, int HighlightsIntensity, int ReduceOthersTo = -1);
		virtual void Apply(Image&) const override;

	private:
		int m_RangeA;
		int m_RangeB;
		int m_HighlightsIntensity;
		int m_ReduceOthersTo;
	};
}