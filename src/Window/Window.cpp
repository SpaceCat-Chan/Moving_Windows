#include "Window.hpp"

SDL_GLContext Window::m_Context;

Window::Window(Window &&Move)
{
	m_Window = Move.m_Window;

	Move.m_Window = nullptr;
	Move.m_Context = nullptr;
}

const Window &Window::operator=(Window &&Move)
{
	Destroy();

	m_Window = Move.m_Window;

	Move.m_Window = nullptr;

	return *this;
}

void Window::Destroy()
{
	if (m_Window)
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}
}
void Window::Load(std::string WindowName, long W, long H)
{
	Destroy();
	m_Window = SDL_CreateWindow(
	    WindowName.c_str(),
	    SDL_WINDOWPOS_UNDEFINED,
	    SDL_WINDOWPOS_UNDEFINED,
	    W,
	    H,
	    SDL_WINDOW_OPENGL);
	if (m_Window == nullptr)
	{
		std::cerr << "unable to create window: " << WindowName
		          << " with {W, H}: {" << W << ", " << H
		          << "}\nSDL_Error: " << SDL_GetError() << '\n';
		throw;
	}
}

void Window::Bind()
{
	if (m_Context == nullptr)
	{
		m_Context = SDL_GL_CreateContext(m_Window);
		if (m_Context == nullptr)
		{
			std::cerr << "Failed to create Context\nSDL_Error: "
			          << SDL_GetError() << '\n';
			throw;
		}
	}
	if (SDL_GL_MakeCurrent(m_Window, m_Context) < 0)
	{
		std::cerr << "unable to bind Context\nSDL_Error: " << SDL_GetError()
		          << '\n';
	}
}

void Window::ChangeSize(long W, long H) { SDL_SetWindowSize(m_Window, W, H); }

void Window::Move(long X, long Y) { SDL_SetWindowPosition(m_Window, X, Y); }

void Window::MoveAccountForBorder(long X, long Y)
{
	int top, left;
	SDL_GetWindowBordersSize(m_Window, &top, &left, nullptr, nullptr);
	Move(X + left, Y + top);
}

SDL_Rect Window::GetBounds()
{
	SDL_Rect bounds;
	SDL_GetWindowPosition(m_Window, &bounds.x, &bounds.y);
	SDL_GetWindowSize(m_Window, &bounds.w, &bounds.h);
	return bounds;
}

SDL_Rect Window::GetBoundsWithBorder()
{
	auto bounds = GetBounds();
	int top, left, bottom, right;
	SDL_GetWindowBordersSize(m_Window, &top, &left, &bottom, &right);
	bounds.x -= left;
	bounds.w += left + right;
	bounds.y -= top;
	bounds.h += top + bottom;
	return bounds;
}
