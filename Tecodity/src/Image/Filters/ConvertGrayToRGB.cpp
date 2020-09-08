#include "pch.h"
#include "ConvertGrayToRGB.h"

#include <cmath>

namespace Tecodity {

	void ConvertGrayToRGB::Apply(Image& image) const
	{
		int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		image.EnableNegativeValues();
		while (i--)
		{
			image[i] = m_Transformation(image[i].GetValue()).c;
		}

		image.SetFormat(ImageFormat::RGB);
	}
}
