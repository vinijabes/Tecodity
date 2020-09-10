#pragma once

#include "FilterInterface.h"

namespace Tecodity {
	class RGBFilterWrapper : public FilterInterface
	{
	protected:
		std::shared_ptr<FilterInterface> m_Filter;
	public:
		RGBFilterWrapper(std::shared_ptr<FilterInterface> filter)
			: m_Filter(filter)
		{};
		
		virtual void Apply(Image& input) const override
		{
			if (input.GetFormat() == ImageFormat::RGB)
			{
				auto r = input.GetRChannelImage();
				auto g = input.GetGChannelImage();
				auto b = input.GetBChannelImage();

				m_Filter->Apply(r);
				m_Filter->Apply(g);
				m_Filter->Apply(b);

				input.SetRChannelFromImage(r);
				input.SetGChannelFromImage(g);
				input.SetBChannelFromImage(b);

			}
			else
			{
				m_Filter->Apply(input);
			}
		}
	};
}
