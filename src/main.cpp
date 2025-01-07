#include "pch.h"

#include "Shader/Shader.h"
#include "VertexBuffer/VertexBuffer.h"
#include "VertexArray/VertexArray.h"
#include "Input/Input.h"

#include "Camera/Camera.h"
#include "Star/Map.h"
#include "Star/Player.h"
#include "Timer/Timer.h"
#include "GUI/ImGuiWrapper.h"


constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 800;


#undef main
int main(int argc, char* argv[])
{
	srand((unsigned int)time(0));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG_ERROR("SDL_Init(SDL_INIT_VIDEO) failed: {0}", SDL_GetError());
		return -1;
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) < 0)
	{
		LOG_ERROR("SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) failed: {0}", SDL_GetError());
		return -1;
	}

	if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6))
	{
		LOG_ERROR("SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6) failed: {0}", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	SDL_Window* window = SDL_CreateWindow("A_Star_Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window)
	{
		LOG_ERROR("SDL_CreateWindow failed: {0}", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	SDL_GLContext context;
	context = SDL_GL_CreateContext(window);
	if (!context)
	{
		LOG_ERROR("SDL_GL_CreateContext failed: {0}", SDL_GetError());
		return -1;
	}

	if (SDL_GL_MakeCurrent(window, context) < 0)
	{
		LOG_ERROR("SDL_GL_MakeCurrent failed: {0}", SDL_GetError());
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		LOG_ERROR("gladLoadGLLoader failed: {0}", SDL_GetError());
		return -1;
	}

	const GLubyte* version = glGetString(GL_VERSION);
	if (version)
	{
		std::string versionStr(reinterpret_cast<const char*>(version));
		LOG_INFO("OpenGL version: {0}", versionStr);
	}

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		LOG_ERROR("Source: {0}, Type: {1}, Id: {2}, Severity: {3}, Length: {4}, Message: {5}", source, type, id, severity, length, message);
	}, nullptr);

	SDL_GL_SetSwapInterval(0);

	ImGuiWrapper::Init(window, "#version 460 core");

	Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);
	Map map(800.0f, 800.0f, 32.0f, 32.0f);

	Player player(map, camera);

	Timer mainTickTimer;

	float mainTickDeltaTime = 0.0f;

	bool bWantsToQuit = false;
	while (!bWantsToQuit)
	{
		const float lastTickDeltaTime = mainTickTimer.GetElapsedTime();
		mainTickDeltaTime = lastTickDeltaTime;

		mainTickTimer.ResetTimer();

		//LOG_INFO("deltaTime ms: {0}", Timer::ConvertSecondsToMS(mainTickDeltaTime));
		//LOG_INFO("FPS: {0}", uint32_t(1000.0f / Timer::ConvertSecondsToMS(mainTickDeltaTime)));

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);

			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				if (event.window.windowID == SDL_GetWindowID(window))
				{
					bWantsToQuit = true;
				}
			}
		}

		// no layer support, sry :-(. Mouse click events do propagate through ImGUI
		// There were no intention to fix it in the current project
		// this project is about path finding, not layer system creation
		//
		// Just try to keep ImGui windows outside game viewport
		//

		camera.Update(mainTickDeltaTime);
		map.Update(mainTickDeltaTime);
		player.Update(mainTickDeltaTime);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		map.Draw(camera);

		ImGuiWrapper::NewFrame();

		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::ShowDemoWindow();

		player.ImGuiFrameUpdate();
		camera.ImGuiFrameUpdate();

		map.ImGuiFrameUpdate();
		if (ImGui::Button("Recreate Tile Map", { 140.0f, 20.0f }))
		{
			map.RecreateMap();
			player.Reset();
		}

		ImGuiWrapper::EndFrameAndRender();

		SDL_GL_SwapWindow(window);
	}

	ImGuiWrapper::Cleanup();

	return 0;
}
