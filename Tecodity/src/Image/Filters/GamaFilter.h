#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class GamaFilter : public FilterInterface
	{
	public:
		GamaFilter(double c, double gama);
		virtual void Apply(Image&) const override;

	private:
		double m_C;
		double m_Gama;
	};
}
