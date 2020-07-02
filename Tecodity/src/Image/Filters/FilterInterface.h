#pragma once

#include "Image/Image.h"

namespace Tecodity {
	class FilterInterface
	{
	public:
		virtual void Apply(Image& ) const = 0;
	};
}