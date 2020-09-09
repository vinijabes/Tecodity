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


	class Pipeline;

	class StepInput
	{
	protected:
		std::vector<std::string> m_Params;
	public:
		void Add(std::string param)
		{
			m_Params.push_back(param);
		}

		bool Has(int pos) const
		{
			return pos < m_Params.size();
		}

		std::string Get(int pos) const
		{
			return m_Params[pos];
		}

		int GetNumber(int pos) const
		{
			std::string s = m_Params[pos];
			return std::stoi(s);
		}

		bool IsNumber(int pos) const
		{
			std::string s = m_Params[pos];
			return !s.empty() && std::find_if(s.begin(),
				s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
		}
	};


	class AbstractPipelineStep
	{
	private:
		Pipeline* m_Pipeline;
	public:
		virtual void SetPipeline(Pipeline* pipeline) { m_Pipeline = pipeline;  }
		virtual Pipeline* GetPipeline() { return m_Pipeline; }
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
		virtual void SetPipeline(Pipeline* pipeline) { AbstractPipelineStep::SetPipeline(pipeline); }
		virtual Pipeline* GetPipeline() { return AbstractPipelineStep::GetPipeline(); }

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
		virtual void SetPipeline(Pipeline* pipeline) { AbstractPipelineStep::SetPipeline(pipeline); }
		virtual Pipeline* GetPipeline() { return AbstractPipelineStep::GetPipeline(); }

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
		virtual void SetPipeline(Pipeline* pipeline) { SrcPipelineStep::SetPipeline(pipeline); }
		virtual Pipeline* GetPipeline() { return SrcPipelineStep::GetPipeline(); }
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
		std::function<std::shared_ptr<FilterInterface>(const StepInput&, Pipeline&)> m_FilterGenerator;
		StepInput m_StepInput;
	public:
		FilterPipelineStep(std::shared_ptr<FilterInterface> filter)
			: m_Filter(filter)
		{}

		FilterPipelineStep(std::function<std::shared_ptr<FilterInterface>(const StepInput&, Pipeline&)> generator, StepInput input)
			: m_FilterGenerator(generator), m_StepInput(input)
		{}

		virtual void Execute() override
		{
			if (!m_Filter)
			{
				m_Filter = m_FilterGenerator(m_StepInput, *this->GetPipeline());
			}

			if (m_Filter != nullptr)
			{
				m_Input = std::make_shared<Image>(Image::ApplyFilter(*m_Result, *m_Filter));
			}
		};
	};

	class Pipeline
	{
		std::map<std::string, std::shared_ptr<AbstractPipelineStep>> m_Steps;
		std::map<std::string, std::shared_ptr<SrcPipelineStep>> m_SrcSteps;
		std::map<std::string, std::shared_ptr<SrcPipelineStep>> m_InitialSteps;

		std::shared_ptr<SrcPipelineStep> m_LastSrcStep;
	public:
		std::shared_ptr<Image> GetInput(std::string src)
		{
			if (m_SrcSteps.find(src) != m_SrcSteps.end())
			{
				auto srcstep = m_SrcSteps[src];
				auto srcsinkstep = std::dynamic_pointer_cast<SinkSrcPipelineStep>(srcstep);

				if (srcstep->Get() != nullptr) return srcstep->Get();
				if (srcsinkstep == nullptr)
				{
					srcstep->Execute();
					return srcstep->Get();
				}
				else if(srcsinkstep->GetPreviousStep() == nullptr)
				{
					srcsinkstep->Execute();
					return srcstep->Get();
				}
			}
			else {
				auto step = std::make_shared<LoadImagePipelineStep>(src);
				step->Execute();
				m_SrcSteps[src] = step;
				return step->Get();
			}
		}

		void AddStep(std::shared_ptr<FilterPipelineStep> step)
		{
			AddStep(random_string(16), std::static_pointer_cast<AbstractPipelineStep>(std::static_pointer_cast<SrcPipelineStep>(step)));
		}

		void AddStep(std::shared_ptr<AbstractPipelineStep> step)
		{
			AddStep(random_string(16), step);
		}

		void AddStep(std::string name, std::shared_ptr<AbstractPipelineStep> step)
		{
			m_Steps[name] = step;
			step->SetPipeline(this);

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

		void Execute()
		{
			try
			{
				std::stack<std::shared_ptr<AbstractPipelineStep>> executingSteps;

				for (auto it = m_InitialSteps.begin(); it != m_InitialSteps.end(); it++)
				{
					executingSteps.push(std::static_pointer_cast<AbstractPipelineStep>(it->second));
				}

				while (!executingSteps.empty())
				{
					std::shared_ptr<AbstractPipelineStep> current_step = executingSteps.top();
					executingSteps.pop();

					current_step->SetPipeline(this);
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
			}
			catch (const char* err)
			{
				std::cerr << err << std::endl;
			}
		}
	};

	class PipelineBuilder
	{
	private:
		std::map<std::string, std::function<std::shared_ptr<FilterInterface>(const StepInput&, Pipeline&)>> m_Factory;
	public:
		template <typename T>
		void RegisterFactory(std::string factoryName, std::function<T* (const StepInput&, Pipeline&)> inputPopulator)
		{
			m_Factory[factoryName] = [inputPopulator](const StepInput& input, Pipeline& pipeline) {
				std::shared_ptr<T> filter = std::shared_ptr<T>(inputPopulator(input, pipeline));
				return std::static_pointer_cast<FilterInterface>(filter);
			};
		}

		Pipeline Build()
		{
			Pipeline x;
			StepInput i;
			i.Add("input");
			i.Add("input2");
			i.Add("input3");
			x.AddStep("input", std::make_shared<LoadImagePipelineStep>("weld-original.pgm"));
			x.AddStep("input2", std::make_shared<LoadImagePipelineStep>("weld-original.pgm"));
			x.AddStep("input3", std::make_shared<LoadImagePipelineStep>("weld-original.pgm"));
			x.AddStep(std::make_shared<FilterPipelineStep>(m_Factory["ConvertGrayToRGB"], i));
			x.AddStep(std::make_shared<FilterPipelineStep>(m_Factory["MergeToRGB"], i));
			x.AddStep(std::make_shared<SaveImagePipelineStep>("testepipeline.ppm"));
			return x;
		};
	};
}