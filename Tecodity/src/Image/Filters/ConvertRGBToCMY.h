#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class ConvertRGBToCMY : public FilterInterface
	{
	public:
		ConvertRGBToCMY() = default;
		virtual void Apply(Image&) const override;
	};
}
