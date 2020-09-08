#include "pch.h"
#include "ConvertRGBToCMY.h"

#include <cmath>

namespace Tecodity {

	void ConvertRGBToCMY::Apply(Image& image) const
	{
		if (image.GetFormat() != ImageFormat::RGB) return;

		int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		image.EnableNegativeValues();
		while (i--)
		{
			RGBColor color = { image[i].GetValue() };
			color.rgb.r = 255 - color.rgb.r;
			color.rgb.g = 255 - color.rgb.g;
			color.rgb.b = 255 - color.rgb.b;
			image[i] = color.c;
		}

		image.SetFormat(ImageFormat::CMY);
	}
}
