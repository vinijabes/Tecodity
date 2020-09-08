#include "pch.h"
#include "PGM.h"

namespace Tecodity {
	Matrix<int>& PGM::Load(Matrix<int>& matrix, PBMReader& input)
	{
		auto data = matrix.GetData();
		int i = matrix.GetWidth() * matrix.GetHeight();
		while (i--)
		{
			*data++ = input.Next();
		}

		return matrix;
	}

	void PGM::Save(const Matrix<int>& matrix, PBMWriter& output)
	{
		int i = matrix.GetHeight() * matrix.GetWidth();
		auto data = matrix.GetData();
		while (i--)
		{
			int value = *data++;

			if (value < 0) value = 0;
			else if (value > 255) value = 255;
			output.Write(value);
			if (i % matrix.GetWidth() == 0)output.NewLine();
		}
	}
}
