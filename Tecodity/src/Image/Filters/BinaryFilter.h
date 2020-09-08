#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class BinaryFilter : public FilterInterface
	{
	public:
		BinaryFilter() = default;
		virtual void Apply(Image&) const override;
	};
}
