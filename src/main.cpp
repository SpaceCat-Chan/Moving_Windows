#include <chrono>
#include <iostream>
#include <thread>

#include <GL/glew.h>
#include <SDL.h>
#include <glm/ext.hpp>

#include "Window/Window.hpp"

void GLAPIENTRY MessageCallback(
    GLenum,
    GLenum type,
    GLuint,
    GLenum severity,
    GLsizei,
    const GLchar *message,
    const void *)
{
	// SDL_SetRelativeMouseMode(SDL_TRUE);
	if (type == 0x8251 || type == 0x8250)
	{
		return;
	}
	std::cerr << "GL CALLBACK: "
	          << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
	          << " type = " << std::hex << type << ", severity = " << severity
	          << ", message = " << message << '\n';
}

using steady_clock = std::chrono::steady_clock;
using namespace std::literals;

int main()
{
	Window backup{};

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(
	    SDL_GL_CONTEXT_PROFILE_MASK,
	    SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	backup.Load("Emergancy Window", 600, 600);
	backup.Bind();

	if (auto err = glewInit(); err != GLEW_OK)
	{
		std::cerr << "Unable to init glew: " << glewGetErrorString(err)
		          << std::endl;
	}
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);

	SDL_Rect Screen;
	SDL_GetDisplayUsableBounds(1, &Screen);

	double current_time;
	Window moving_window;
	moving_window.Load("Moving", 400, 400);

	steady_clock::time_point last_frame;
	bool quit = false;
	while (!quit)
	{
		auto this_frame = steady_clock::now();
		auto dt = std::chrono::duration<double>{last_frame - this_frame};
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE
				    && backup.is_id(event.window.windowID))
				{
					quit = true;
				}
			}
		}

		current_time += dt.count();
		glm::dvec2 circle_position{
		    glm::cos(current_time),
		    glm::sin(current_time)};
		circle_position *= 0.5;
		auto bounds = moving_window.GetBoundsWithBorder();
		circle_position *= glm::dvec2{
		    (1 - (bounds.w / (double)Screen.w)) * Screen.w,
		    (1 - (bounds.h / (double)Screen.h)) * Screen.h};
		circle_position
		    += glm::dvec2{Screen.x + Screen.w / 2 - bounds.w/2, Screen.y + Screen.h / 2 - bounds.h/2};

		moving_window.MoveAccountForBorder(circle_position.x, circle_position.y);

		last_frame = this_frame;
		std::this_thread::sleep_until(this_frame + 16ms);
	}
}
