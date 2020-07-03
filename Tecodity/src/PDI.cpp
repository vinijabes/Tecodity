#include "pch.h"

#include <iostream>
#include <time.h>

#include "Math/Matrix.h"
#include "Image/PBM/PBMMatrix.h"

#include "Image/Filters/NegativeFilter.h"
#include "Image/Filters/HorizontalFlipFilter.h"
#include "Image/Filters/GamaFilter.h"
#include "Image/Filters/HistogramEqualizationFilter.h"
#include "Image/Filters/IntensityLevelSlicingFilter.h"

int main()
{
	std::cout << "INIT" << std::endl;
	Tecodity::Image lena = Tecodity::PBMMatrix::Load("lena640.pgm");
	auto lenaFlipped = std::move(Tecodity::Image::ApplyFilter(lena, Tecodity::HorizontalFlipFilter()));
	Tecodity::PBMMatrix::Save("lenaFlipped640.pgm", lenaFlipped);
		
	Tecodity::Image phist = Tecodity::PBMMatrix::Load("phistf1.pgm");
	auto phistequalized = std::move(Tecodity::Image::ApplyFilter(phist, Tecodity::HistogramEqualizationFilter(256)));
	Tecodity::PBMMatrix::Save("phistEqualized640.pgm", phistequalized);

	Tecodity::Image ctskull = Tecodity::PBMMatrix::Load("ctskull-256.pgm");
	auto levelSlice = std::move(Tecodity::Image::ApplyFilter(ctskull, Tecodity::IntensityLevelSlicingFilter(100, 150, 120, 0)));
	Tecodity::PBMMatrix::Save("ctskullHighlighted-256.pgm", levelSlice);

	Tecodity::Image spine = Tecodity::PBMMatrix::Load("spine.pgm");
	auto gamaCorrection = std::move(Tecodity::Image::ApplyFilter(spine, Tecodity::GamaFilter(1, 0.6)));
	Tecodity::PBMMatrix::Save("spineGama.pgm", gamaCorrection);
}