#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class HorizontalFlipFilter : public FilterInterface
	{
	public:
		HorizontalFlipFilter() = default;
		virtual void Apply(Image&) const override;
	};
}
