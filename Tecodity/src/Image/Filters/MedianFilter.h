#pragma once

#include "FilterInterface.h"
#include "../../Math/Matrix.h"

namespace Tecodity {
	class MedianFilter : public FilterInterface
	{
	private:
		Matrix<int> m_Filter;

	public:
		MedianFilter(const Matrix<int>& filter)
			: m_Filter(filter)
		{}

		virtual void Apply(Image&) const override;
	};
}