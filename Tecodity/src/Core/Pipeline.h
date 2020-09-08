#pragma once

#include <vector>
#include <map>
#include "Image/Image.h"

namespace Tecodity {

	std::string random_string(size_t length)
	{
		auto randchar = []() -> char {
			const char charset[] =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";
			const size_t max_index = (sizeof(charset) - 1);
			return charset[rand() % max_index];
		};
		std::string str(length, 0);
		std::generate_n(str.begin(), length, randchar);
		return str;
	}

	class StepInput
	{

	};

	class AbstractPipelineStep
	{
	public:
		virtual void Execute() = 0;
	};

	class SrcPipelineStep;
	class SinkPipelineStep;

	class SrcPipelineStep : public AbstractPipelineStep, public std::enable_shared_from_this<SrcPipelineStep>
	{
	protected:
		std::shared_ptr<Image> m_Input;
		std::vector<std::shared_ptr<SinkPipelineStep>> m_NextSteps;
	public:
		SrcPipelineStep()
			: m_Input(nullptr)
		{}
		std::shared_ptr<Image> Get() { return m_Input; };

		void AddNextStep(std::shared_ptr<SinkPipelineStep> step);
		std::vector<std::shared_ptr<SinkPipelineStep>> GetNextSteps() { return m_NextSteps; }
	};

	class SinkPipelineStep : public AbstractPipelineStep
	{
	protected:
		std::shared_ptr<Image> m_Result;
		std::shared_ptr<SrcPipelineStep> m_PreviousStep;
	public:
		SinkPipelineStep()
			: m_Result(nullptr)
		{
		}

		void Set(std::shared_ptr<Image> image) 
		{ 
			if (image == nullptr) throw "Null image";
			m_Result = image; 
		}

		void SetPreviousStep(std::shared_ptr<SrcPipelineStep> step) { m_PreviousStep = step; }
		std::shared_ptr<SrcPipelineStep> GetPreviousStep() { return m_PreviousStep; }
	};

	void SrcPipelineStep::AddNextStep(std::shared_ptr<SinkPipelineStep> step)
	{
		if (step->GetPreviousStep() != nullptr) return;
		step->SetPreviousStep(shared_from_this());
		m_NextSteps.push_back(step);
	}

	class SinkSrcPipelineStep : public SrcPipelineStep, public SinkPipelineStep
	{
	public:
		virtual void Execute() = 0;
	};

	class LoadImagePipelineStep : public SrcPipelineStep
	{
	protected:
		std::string m_Path;
	public:
		LoadImagePipelineStep(std::string path)
			: m_Path(path) {}

		virtual void Execute() override
		{
			m_Input = std::make_shared<Image>(PBMMatrix::Load(m_Path));
		}
	};

	class SaveImagePipelineStep : public SinkPipelineStep
	{
	protected:
		std::string m_Path;
	public:
		SaveImagePipelineStep(std::string path)
			: m_Path(path) {}
		virtual void Execute() override
		{
			PBMMatrix::Save(m_Path, *m_Result, m_Result->GetFormat() == ImageFormat::RGB ? PBMFormat::P3 : PBMFormat::P2);
		}
	};

	class FilterPipelineStep : public SinkSrcPipelineStep
	{
	private:
		std::shared_ptr<FilterInterface> m_Filter;
	public:
		FilterPipelineStep(std::shared_ptr<FilterInterface> filter)
			: m_Filter(filter)
		{}

		virtual void Execute() override
		{
			m_Input = std::make_shared<Image>(Image::ApplyFilter(*m_Result, *m_Filter));
		};
	};

	class Pipeline
	{
		std::map<std::string, std::shared_ptr<AbstractPipelineStep>> m_Steps;
		std::map<std::string, std::shared_ptr<SrcPipelineStep>> m_SrcSteps;
		std::map<std::string, std::shared_ptr<SrcPipelineStep>> m_InitialSteps;

		std::shared_ptr<SrcPipelineStep> m_LastSrcStep;
	public:
		void AddStep(std::shared_ptr<AbstractPipelineStep> step)
		{
			AddStep(random_string(16), step);
		}

		void AddStep(std::string name, std::shared_ptr<AbstractPipelineStep> step)
		{
			m_Steps[name] = step;

			auto sink = std::dynamic_pointer_cast<SinkPipelineStep>(step);

			if (sink != nullptr)
			{
				if (m_LastSrcStep != nullptr)
				{
					m_LastSrcStep->AddNextStep(sink);
				}
			}

			if (std::dynamic_pointer_cast<SrcPipelineStep>(step) != nullptr)
			{
				auto src = std::static_pointer_cast<SrcPipelineStep>(step);
				m_SrcSteps[name] = src;
				if (std::dynamic_pointer_cast<SinkSrcPipelineStep>(src) == nullptr)
				{
					m_InitialSteps[name] = src;
				}

				m_LastSrcStep = src;
			}
		}

		Image Execute(const Image& input)
		{
			std::shared_ptr<Image> current = std::make_shared<Image>(input);

			std::stack<std::shared_ptr<AbstractPipelineStep>> executingSteps;

			for (auto it = m_InitialSteps.begin(); it != m_InitialSteps.end(); it++)
			{
				executingSteps.push(std::static_pointer_cast<AbstractPipelineStep>(it->second));
			}

			while (!executingSteps.empty())
			{
				std::shared_ptr<AbstractPipelineStep> current_step = executingSteps.top();
				executingSteps.pop();

				auto srcstep = std::dynamic_pointer_cast<SrcPipelineStep>(current_step);
				auto sinkstep = std::dynamic_pointer_cast<SinkPipelineStep>(current_step);

				current_step->Execute();

				if (srcstep != nullptr)
				{
					for (auto s : srcstep->GetNextSteps())
					{
						auto input = srcstep->Get();
						s->Set(input);
						executingSteps.push(s);
					}
				}

			}

			//for (auto it = m_Steps.begin(); it != m_Steps.end(); it++)
			//{
			//	auto iostep = std::dynamic_pointer_cast<SinkSrcPipelineStep>(std::dynamic_pointer_cast<SrcPipelineStep>(it->second));
			//	if (iostep != nullptr)
			//	{
			//		iostep->Set(current);
			//		iostep->Execute();
			//		current = iostep->Get();
			//	}
			//}

			return *current;
		}
	};

	class PipelineBuilder
	{
	private:
		std::map<std::string, std::function<std::shared_ptr<AbstractPipelineStep>(const StepInput&)>> m_Factory;
	public:
		template <typename T>
		void RegisterFactory(std::string factoryName, std::function<T* (const StepInput&)> inputPopulator)
		{
			m_Factory[factoryName] = [inputPopulator](const StepInput& input) {
				std::shared_ptr<T> filter = std::shared_ptr<T>(inputPopulator(input));
				return std::static_pointer_cast<AbstractPipelineStep>(std::static_pointer_cast<SrcPipelineStep>(std::make_shared<FilterPipelineStep>(filter)));
			};
		}

		Pipeline Build()
		{
			Pipeline x;
			x.AddStep(std::make_shared<LoadImagePipelineStep>("weld-original.pgm"));
			x.AddStep(m_Factory["teste"](StepInput()));
			x.AddStep(std::make_shared<SaveImagePipelineStep>("testepipeline.ppm"));
			return x;
		};
	};
}