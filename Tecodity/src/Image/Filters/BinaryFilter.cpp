#include "pch.h"
#include "BinaryFilter.h"

#include <cmath>

namespace Tecodity {
	void BinaryFilter::Apply(Image& image) const
	{
		int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		while (i--)
		{
			image[i] = image[i].GetValue() > 60 ? 255 : 0;
		}
	}
}
