#include "pch.h"
#include "GamaFilter.h"

namespace Tecodity {
	GamaFilter::GamaFilter(double c, double gama)
		: m_C(c), m_Gama(gama)
	{
	}

	void GamaFilter::Apply(Image& image) const
	{
		int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
		while (i--)
		{
			image[i] = (std::pow(image[i].GetValue()/255.0, this->m_Gama) * this->m_C) * 255;
		}
	}
}
