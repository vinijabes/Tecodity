#include "pch.h"
#include "NegativeFilter.h"

namespace Tecodity {
	void NegativeFilter::Apply(Image& image) const
	{
		int* values = image.GetMatrix().GetData();
		
		int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		while (i--)
		{
			image[i] = 255 - image[i].GetValue();
		}
	}
}