#include "pch.h"
#include "Application.h"
#include "Common.h"

Application *Application::m_application = nullptr;

Application::Application()
{
}

void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError());
		exit(-1);
	}

	//setting openGL version to 4.2 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating window
	m_window = SDL_CreateWindow("GP3-Lab-1", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight,
		SDL_WINDOW_OPENGL);

	SDL_CaptureMouse(SDL_TRUE);
}

void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	CHECK_GL_ERROR();

	SDL_GL_SetSwapInterval(1);

	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::Loop()
{
	m_appState = AppState::RUNNING;

	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	while (m_appState != AppState::QUITTING)
	{
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;
			case SDL_KEYDOWN:
				//TODO: quit app when user presses escape
				break;
			}
		}

		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime = (float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks - prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		//TODO: print delta time using log

		SDL_GL_SwapWindow(m_window);
	}
}

void Application::Quit()
{
	//Close SDL
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
}


Application::~Application()
{
}

Application * Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}
