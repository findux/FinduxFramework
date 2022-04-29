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

GLuint VBO, VAO;

App::App()
{
	isRunning = false;
	isDebug = false;
	
}

App::~App()
{
	delete shader;
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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

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

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}

	// Define the viewport dimensions
	glViewport(0, 0, windowWidth, windowHeight);

	GLfloat vertices[] =
	{
		// position				//color
		-0.5f , -0.5f , 0.0f,	1.0f, 0.0f,0.0f,
		0.5f , -0.5f , 0.0f,	0.0f, 1.0f,0.0f,
		0.0f , 0.5f, 0.0f,		0.0f, 0.0f,1.0f
	};

	//must be after GLEW init
	shader = new Shader("core.vs", "core.fs");
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


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

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


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
	// Clear the colorbuffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	shader->use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES,0,3);
	glBindVertexArray(0);

	// draw OpenGL

	SDL_GL_SwapWindow(window);
}
