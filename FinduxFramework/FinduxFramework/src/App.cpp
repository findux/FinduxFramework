#include "App.h"

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>
#include <iostream>
#include <fstream>


int App::windowWidth;
int App::windowHeight;

App::App()
{
	isRunning = false;
	isDebug = false;
}

App::~App()
{
}

void App::initialize()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		//Logger::Err("Error initialzing SDL.");
		return;
	}

	if (TTF_Init() != 0)
	{
		//Logger::Err("Error initialzing SDL TTF.");
		return;
	}

	/* Request opengl 3.2 context.
	* SDL doesn't have the ability to choose which profile at this time of writing,
	* but it should default to the core profile */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	/* Turn on double buffering with a 24bit Z buffer.
	 * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = 800; //displayMode.w;
	windowHeight = 600;//  displayMode.h;
	window = SDL_CreateWindow(
		"Hello world SDL.",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
	);
	if (!window)
	{
		//Logger::Err("Error creating SDL window.");
		return;
	}

	/* Create our opengl context and attach it to our window */
	mainContext = SDL_GL_CreateContext(window);

	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);


	//imgui
	//ImGui::CreateContext();
	//ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);


	isRunning = true;
}

void App::run()
{
	setup();
	while (isRunning)
	{
		processInput();
		update();
		render();
	}
}

void App::destroy()
{
	//ImGuiSDL::Deinitialize();
	//ImGui::DestroyContext();
	/* Delete our opengl context, destroy our window, and shutdown SDL */
	SDL_GL_DeleteContext(mainContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void App::setup()
{
}

void App::processInput()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		//core sdl
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
			if (sdlEvent.key.keysym.sym == SDLK_d)
			{
				isDebug = !isDebug;
			}
			break;
		default:
			break;
		}
	}
}

void App::update()
{

}

void App::render()
{
	//SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	//SDL_RenderClear(renderer);
	//
	//SDL_RenderPresent(renderer);
	/* This makes our buffer swap syncronized with the monitor's vertical refresh */
	SDL_GL_SetSwapInterval(1);

	/* Clear our buffer with a red background */
	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	/* Swap our back buffer to the front */
	SDL_GL_SwapWindow(window);
	/* Wait 2 seconds */
	SDL_Delay(2000);

	/* Same as above, but green */
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);
	SDL_Delay(2000);

	/* Same as above, but blue */
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(window);
	SDL_Delay(2000);
}
