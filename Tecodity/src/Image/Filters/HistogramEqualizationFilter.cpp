#include "pch.h"
#include "HistogramEqualizationFilter.h"

#include <map>
#include <cmath>

namespace Tecodity {
	HistogramEqualizationFilter::HistogramEqualizationFilter(int grayScale)
		: m_GrayScale(grayScale)
	{}

	void HistogramEqualizationFilter::Apply(Image& image) const
	{
		auto graymap = std::map<int, int>();
		auto probmap = std::map<int, double>();
		auto transformedGrayMap = std::map<int, int>();

		for (int i = 0; i < 255; ++i)
		{
			graymap[i] = 0;
		}
		
		int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		while (i--)
		{
			graymap[image[i].GetValue()]++;
		}

		int size = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		for (int i = 0; i < 255; ++i)
		{
			probmap[i] = (double)graymap[i]/(double)size;

			if (i > 0)
			{
				probmap[i] += probmap[i - 1];
			}

			transformedGrayMap[i] = std::round((this->m_GrayScale - 1) * probmap[i]);
		}

		i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		while (i--)
		{
			image[i] = transformedGrayMap[image[i].GetValue()];
		}
	}
}
