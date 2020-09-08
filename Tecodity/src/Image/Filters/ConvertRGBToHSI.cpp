#include "pch.h"
#include "ConvertRGBToHSI.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

namespace Tecodity {

	void ConvertRGBToHSI::Apply(Image& image) const
	{
		if (image.GetFormat() != ImageFormat::RGB) return;

		int count = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		image.EnableNegativeValues();
		while (count--)
		{
			RGBColor color = { image[count].GetValue() };

			double r = color.rgb.r / (double)(color.rgb.r + color.rgb.g + color.rgb.b);
			double g = color.rgb.g / (double)(color.rgb.r + color.rgb.g + color.rgb.b);
			double b = color.rgb.b / (double)(color.rgb.r + color.rgb.g + color.rgb.b);

			double h, s, i;
			double num = (0.5 * (2 * r - g - b));
			double den = std::sqrt(std::pow(r-g, 2) + (r-b)*(g-b));
			h = std::acos(num/den);

			if (b > g)
			{
				h = 2 * M_PI - h;
			}

			s = 1 - 3.0 * std::min(std::min(r, g), b);
			i = (r + g + b) / (3.0 * 255.0);

			color.rgb.r = h/(2*M_PI) * 255;
			color.rgb.g = s * 255;
			color.rgb.b = i * 255;

			image[count] = color.c;
		}

		image.SetFormat(ImageFormat::HSI);
	}
}
