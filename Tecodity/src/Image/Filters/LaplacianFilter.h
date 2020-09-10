#pragma once

#include "FilterInterface.h"
#include "../../Math/Matrix.h"

namespace Tecodity {
	class LaplacianFilter : public FilterInterface
	{
	private:
		Matrix<int> m_Filter;

	public:
		LaplacianFilter(const Matrix<int>& filter)
			: m_Filter(filter)
		{}

		virtual void Apply(Image&) const override;

		static Matrix<int> Positive4()
		{
			auto x = Matrix<int>(3, 3);
			*(x.GetData() + 1) = -1;
			*(x.GetData() + 3) = -1;
			*(x.GetData() + 4) =  4;
			*(x.GetData() + 5) = -1;
			*(x.GetData() + 7) = -1;
			return x;
		}

		static Matrix<int> Negative4()
		{
			auto x = Matrix<int>(3, 3);
			*(x.GetData() + 1) =  1;
			*(x.GetData() + 3) =  1;
			*(x.GetData() + 4) = -4;
			*(x.GetData() + 5) =  1;
			*(x.GetData() + 7) =  1;
			return x;
		}

		static Matrix<int> Positive8()
		{
			auto x = Matrix<int>(3, 3);
			*(x.GetData() + 0) = -1;
			*(x.GetData() + 1) = -1;
			*(x.GetData() + 2) = -1;
			*(x.GetData() + 3) = -1;
			*(x.GetData() + 4) =  8;
			*(x.GetData() + 5) = -1;
			*(x.GetData() + 6) = -1;
			*(x.GetData() + 7) = -1;
			*(x.GetData() + 8) = -1;
			return x;
		}

		static Matrix<int> Negative8()
		{
			auto x = Matrix<int>(3, 3);
			*(x.GetData() + 0) =  1;
			*(x.GetData() + 1) =  1;
			*(x.GetData() + 2) =  1;
			*(x.GetData() + 3) =  1;
			*(x.GetData() + 4) = -8;
			*(x.GetData() + 5) =  1;
			*(x.GetData() + 6) =  1;
			*(x.GetData() + 7) =  1;
			*(x.GetData() + 8) =  1;
			return x;
		}
	};
}