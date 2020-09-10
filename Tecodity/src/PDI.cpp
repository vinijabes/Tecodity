#include "pch.h"

#include <iostream>
#include <time.h>

#include "Math/Matrix.h"
#include "Image/PBM/PBMMatrix.h"

#include "Image/Filters/ConvertRGBToCMY.h"
#include "Image/Filters/ConvertRGBToHSI.h"
#include "Image/Filters/LaplacianFilter.h"
#include "Image/Filters/CustomFilter.h"
#include "Image/Filters/RGBFilterWrapper.h"
#include "Image/Filters/HistogramEqualizationFilter.h"
#include "Image/Filters/IntensityLevelSlicingFilter.h"
#include "Image/Filters/MedianFilter.h"
#include "Image/Filters/SpatialFilter.h"
#include "Image/Filters/BinaryFilter.h"

#include "Core/Pipeline.h"

int main(int argc, char** argv)
{
	std::cout << "INIT" << std::endl;

	Tecodity::PipelineBuilder builder;
	builder.RegisterFactory<Tecodity::RGBFilterWrapper>("EqualizeHistogram", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		int scale = 255;
		if (input.Has(0) && input.IsNumber(0)) scale = input.GetNumber(0);
		if (scale > 255 || scale < 0) throw "(EqualizeHistogram) invalid scale";

		return new Tecodity::RGBFilterWrapper(std::make_shared<Tecodity::HistogramEqualizationFilter>(scale));
		});

	builder.RegisterFactory<Tecodity::RGBFilterWrapper>("IntensityLevelSlicing", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		if (!input.Has(2)) throw "(IntensityLevelSlicing) invalid argument count";
		if (!input.IsNumber(0) || !input.IsNumber(1) || !input.IsNumber(2)) throw "(IntensityLevelSlicing) invalid argument count";
		int rangeA = input.GetNumber(0);
		int rangeB = input.GetNumber(1);
		int newLevel = input.GetNumber(2);
		int othersLevel = -1;

		if (input.Has(3) && input.IsNumber(3)) othersLevel = input.GetNumber(3);

		return new Tecodity::RGBFilterWrapper(std::make_shared<Tecodity::IntensityLevelSlicingFilter>(rangeA, rangeB, newLevel, othersLevel));
		});

	builder.RegisterFactory<Tecodity::RGBFilterWrapper>("MedianFilter", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		int filtersize = 3;
		if (input.Has(0) && input.IsNumber(0)) filtersize = input.GetNumber(0);

		return new Tecodity::RGBFilterWrapper(std::make_shared<Tecodity::MedianFilter>(Tecodity::Matrix<int>(filtersize,filtersize,1)));
		});

	builder.RegisterFactory<Tecodity::RGBFilterWrapper>("SpatialFilter", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		int filtersize = 3;
		if (input.Has(0) && input.IsNumber(0)) filtersize = input.GetNumber(0);

		return new Tecodity::RGBFilterWrapper(std::make_shared<Tecodity::SpatialFilter>(Tecodity::Matrix<int>(filtersize, filtersize, 1)));
	});

	builder.RegisterFactory<Tecodity::RGBFilterWrapper>("BinaryFilter", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::RGBFilterWrapper(std::make_shared<Tecodity::BinaryFilter>());
		});

	builder.RegisterFactory<Tecodity::RGBFilterWrapper>("SumScalar", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::RGBFilterWrapper(std::make_shared<Tecodity::CustomFilter>([&](Tecodity::Image& image) {
			if (!input.Has(0))
			{
				throw "(SumScalar) missing parameters";
			}

			if (!input.IsNumber(0))
			{
				throw "(SumScalar) invalid input";
			}

			image.Add(input.GetNumber(0));

			}));
		});

	builder.RegisterFactory<Tecodity::CustomFilter>("MultiplyScalar", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::CustomFilter([&](Tecodity::Image& image) {
			if (!input.Has(0))
			{
				throw "(MultiplyScalar) missing parameters";
			}

			if (!input.IsNumber(0))
			{
				throw "(MultiplyScalar) invalid input";
			}

			image.Multiply(input.GetNumber(0));

			});
		});

	builder.RegisterFactory<Tecodity::CustomFilter>("SumImage", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::CustomFilter([&](Tecodity::Image& image) {
			if (!input.Has(0))
			{
				throw "(MultiplyScalar) missing parameters";
			}

			auto imageInput = pipeline.GetInput(input.Get(0));

			image.Add(*imageInput);

			});
		});

	builder.RegisterFactory<Tecodity::CustomFilter>("SubImage", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::CustomFilter([&](Tecodity::Image& image) {
			if (!input.Has(0))
			{
				throw "(MultiplyScalar) missing parameters";
			}

			auto imageInput = pipeline.GetInput(input.Get(0));

			image.Sub(*imageInput);

			});
		});

	builder.RegisterFactory<Tecodity::CustomFilter>("MergeToRGB", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::CustomFilter([&](Tecodity::Image& image) {
			if (!input.Has(2))
			{
				throw "(MergeToRGB) missing parameters";
			}

			auto image0 = pipeline.GetInput(input.Get(0));
			auto image1 = pipeline.GetInput(input.Get(1));
			auto image2 = pipeline.GetInput(input.Get(2));

			if (!(image0->GetWidth() == image1->GetWidth() && image0->GetWidth() == image2->GetWidth()) ||
				!(image0->GetHeight() == image1->GetHeight() && image0->GetHeight() == image2->GetHeight())) throw "(MergeToRGB) different image sizes";

			if (image0->GetFormat() != Tecodity::ImageFormat::R) throw "(MergeToRGB) Invalid R input channel";
			if (image1->GetFormat() != Tecodity::ImageFormat::R) throw "(MergeToRGB) Invalid G input channel";
			if (image2->GetFormat() != Tecodity::ImageFormat::R) throw "(MergeToRGB) Invalid B input channel";

			image.SetFormat(Tecodity::ImageFormat::RGB);
			image = *image0;
			image.SetGChannelFromImage(*image1);
			image.SetBChannelFromImage(*image2);
		});
	});

	builder.RegisterFactory<Tecodity::CustomFilter>("GetRChannel", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::CustomFilter([&](Tecodity::Image& image) {
			if (image.GetFormat() != Tecodity::ImageFormat::RGB) throw "(GetRChannel) Invalid input format";

			image = image.GetRChannelImage();
			image.SetFormat(Tecodity::ImageFormat::R);
			});
		});

	builder.RegisterFactory<Tecodity::CustomFilter>("GetGChannel", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::CustomFilter([&](Tecodity::Image& image) {
			if (image.GetFormat() != Tecodity::ImageFormat::RGB) throw "(GetGChannel) Invalid input format";

			image = image.GetGChannelImage();
			image.SetFormat(Tecodity::ImageFormat::R);
			});
		});

	builder.RegisterFactory<Tecodity::CustomFilter>("GetBChannel", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		return new Tecodity::CustomFilter([&](Tecodity::Image& image) {
			if (image.GetFormat() != Tecodity::ImageFormat::RGB) throw "(GetBChannel) Invalid input format";

			image = image.GetBChannelImage();
			image.SetFormat(Tecodity::ImageFormat::R);
			});
		});

	builder.RegisterFactory<Tecodity::RGBFilterWrapper>("Laplacian", [](const Tecodity::StepInput& input, Tecodity::Pipeline& pipeline) {
		Tecodity::Matrix<int> filter;

		if (!input.Has(0))
		{
			filter = Tecodity::LaplacianFilter::Positive4();
		}
		else
		{
			std::string f = input.Get(0);
			if (f == "+4") filter = Tecodity::LaplacianFilter::Positive4();
			else if (f == "-4") filter = Tecodity::LaplacianFilter::Negative4();
			else if (f == "+8") filter = Tecodity::LaplacianFilter::Positive8();
			else if (f == "-8") filter = Tecodity::LaplacianFilter::Negative8();
		}

		return new Tecodity::RGBFilterWrapper(std::make_shared<Tecodity::LaplacianFilter>(filter));
		});

	Tecodity::Pipeline p;
	if (argc >= 2)
	{
		p = builder.BuildFromFile(argv[1]);
	}
	else
	{
		p = builder.BuildFromFile("Teste123.pip");
	}
	p.Execute();
}