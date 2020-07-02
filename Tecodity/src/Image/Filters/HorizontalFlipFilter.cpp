#include "pch.h"
#include "HorizontalFlipFilter.h"

namespace Tecodity {
	void HorizontalFlipFilter::Apply(Image& image) const
	{
		int temp, lastInRow = image.GetMatrix().GetWidth() - 1, rowBase = 0;
		auto data = image.GetMatrix().GetData();

		int height = image.GetMatrix().GetHeight();
		int width = image.GetMatrix().GetWidth();

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width/2; ++x)
			{
				temp = data[rowBase + x];
				data[rowBase + x] = data[rowBase - x + lastInRow];
				data[rowBase - x + lastInRow] = temp;
			}
			rowBase += width;
		}
	}
}