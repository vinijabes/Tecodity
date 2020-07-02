#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class NegativeFilter : public FilterInterface
	{
	public:
		NegativeFilter() = default;
		virtual void Apply(Image&) const override;
	};
}