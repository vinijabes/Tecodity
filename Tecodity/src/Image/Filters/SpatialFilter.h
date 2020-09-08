#pragma once

#include "FilterInterface.h"
#include "../../Math/Matrix.h"

namespace Tecodity {
	class SpatialFilter : public FilterInterface
	{
	private:
		Matrix<int> m_Filter;

	public:
		SpatialFilter(const Matrix<int>& filter)
			: m_Filter(filter)
		{
		}

		virtual void Apply(Image&) const override;
	};
}