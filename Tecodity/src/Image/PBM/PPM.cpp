#include "pch.h"
#include "PPM.h"

namespace Tecodity {
	Matrix<int>& PPM::Load(Matrix<int>& matrix, PBMReader& input)
	{
		auto data = matrix.GetData();
		int i = matrix.GetWidth() * matrix.GetHeight();

		RGBColor color;
		while (i--)
		{
			color.rgb.r = input.Next();
			color.rgb.g = input.Next();
			color.rgb.b = input.Next();
			*data++ = color.c;
		}

		return matrix;
	}

	void PPM::Save(const Matrix<int>& matrix, PBMWriter& output)
	{
		int i = matrix.GetHeight() * matrix.GetWidth();
		auto data = matrix.GetData();
		while (i--)
		{
			RGBColor color = { *data++ };

			if (color.rgb.r < 0) color.rgb.r = 0;
			else if (color.rgb.r > 255) color.rgb.r = 255;
			output.Write(color.rgb.r);

			if (color.rgb.g < 0) color.rgb.g = 0;
			else if (color.rgb.g > 255) color.rgb.g = 255;
			output.Write(color.rgb.g);

			if (color.rgb.b < 0) color.rgb.b = 0;
			else if (color.rgb.b > 255) color.rgb.b = 255;
			output.Write(color.rgb.b);

			if (i % matrix.GetWidth() == 0)output.NewLine();
		}
	}
}
