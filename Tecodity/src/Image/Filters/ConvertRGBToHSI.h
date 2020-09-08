#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class ConvertRGBToHSI : public FilterInterface
	{
	public:
		ConvertRGBToHSI() = default;
		virtual void Apply(Image&) const override;
	};
}
