/**
 * \file Window.hpp
 */

#pragma once

#include <functional>
#include <iostream>
#include <memory>

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

/**
 * \class Window
 * \brief a class that handles one window
 */
class Window
{
	SDL_Window *m_Window = nullptr;
	static SDL_GLContext m_Context;

	friend void SDL_GL_SwapWindow(Window &);

	public:
	Window() = default;

	Window(const Window &Copy) = delete;
	/**
	 * \brief construct a window from moving another window
	 * 
	 * \param Move the window to move from
	 * 
	 * notice: the other window will be cleared
	 */
	Window(Window &&Move);

	const Window &operator=(const Window &Copy) = delete;
	/**
	 * \brief move another window
	 * 
	 * \param Move the window to move
	 * \return a refernce to this window
	 * 
	 * notice: the other window will be cleared
	 */
	const Window &operator=(Window &&Move);

	/**
	 * Destroys the window
	 */
	void Destroy();
	/**
	 * \brief Creates a new info with basic info
	 * 
	 * \param WindowName the name of the window to create
	 * \param W the Width of the window
	 * \param H the Height of the window
	 */
	void Load(std::string WindowName, long W, long H);

	/**
	 * \brief binds this window to be drawn to
	 */
	void Bind();

	void ChangeSize(long W, long H);
	void Move(long X, long Y);
	void MoveAccountForBorder(long X, long Y);
	SDL_Rect GetBounds();
	SDL_Rect GetBoundsWithBorder();



	bool is_id(Uint32 id)
	{
		return m_Window != nullptr && (m_Window == SDL_GetWindowFromID(id));
	}
};

inline void SDL_GL_SwapWindow(Window &window)
{
	SDL_GL_SwapWindow(window.m_Window);
}
