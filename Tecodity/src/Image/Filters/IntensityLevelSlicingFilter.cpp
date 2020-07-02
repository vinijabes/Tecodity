#include "pch.h"
#include "IntensityLevelSlicingFilter.h"

namespace Tecodity {
	IntensityLevelSlicingFilter::IntensityLevelSlicingFilter(int RangeA, int RangeB, int HighlightsIntensity, int ReduceOthersTo)
		: m_RangeA(RangeA), m_RangeB(RangeB), m_HighlightsIntensity(HighlightsIntensity), m_ReduceOthersTo(ReduceOthersTo)
	{}

	void IntensityLevelSlicingFilter::Apply(Image& image) const
	{
		int* values = image.GetMatrix().GetData();

		int width = image.GetMatrix().GetWidth();
		int height = image.GetMatrix().GetHeight();
		int size = width * height;

		for (int i = 0; i < size; ++i)
		{
			if (*(values + i) >= this->m_RangeA && *(values + i) <= this->m_RangeB)
			{
				image[i] = this->m_HighlightsIntensity;
			}
			else if (this->m_ReduceOthersTo > 0)
			{
				image[i] = this->m_ReduceOthersTo;
			}
		}
	}
}
