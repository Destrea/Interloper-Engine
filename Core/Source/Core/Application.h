#pragma once

#include "Layer.h"
#include "Window.h"
#include "Event.h"
#include "Core/Renderer/Framebuffer.h"
#include "Core/Scene/Scene.h"

#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>
#include <set>

#include "Platform/LinuxInput.h"

namespace Core {

	struct ApplicationSpecification
	{
		std::string Name = "Application";
		WindowSpecification WindowSpec;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& specification = ApplicationSpecification());
		~Application();

		void Run();
		void Stop();

		void RaiseEvent(Event& event);

		template<typename TLayer>
		requires(std::is_base_of_v<Layer, TLayer>)
		void PushLayer()
		{
			m_LayerStack.push_back(std::make_unique<TLayer>());
		}

		glm::vec2 GetFramebufferSize() const;

		std::shared_ptr<Window> GetWindow() const {return m_Window;}
		std::shared_ptr<Renderer::Framebuffer> GetFramebuffer() const {return m_Framebuffer;}
		std::shared_ptr<Core::Scene> GetActiveScene() const {return m_ActiveScene; }
		static Application& Get();
		ApplicationSpecification GetSpec() {return m_Specification; }
		static float GetTime();
	private:
		ApplicationSpecification m_Specification;
		std::shared_ptr<Window> m_Window;
		std::shared_ptr<Renderer::Framebuffer> m_Framebuffer;
		bool m_Running = false;

		float limitFPS = 1.0f / 60.0f;

		std::vector<std::unique_ptr<Layer>> m_LayerStack;
		std::shared_ptr<Core::Scene> m_ActiveScene = nullptr;

		friend class Layer;
	};

}
