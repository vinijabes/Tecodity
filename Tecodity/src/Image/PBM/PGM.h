#pragma once

#include "PBMMatrix.h"

namespace Tecodity {
	class PGM : public PBMMatrix
	{
	public:
		static Matrix<int>& Load(Matrix<int>& matrix, PBMReader& input);
		static void Save(const Matrix<int>& matrix, PBMWriter& output);
	};
}