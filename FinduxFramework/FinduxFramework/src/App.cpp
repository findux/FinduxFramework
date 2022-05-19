#include "App.h"

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_sdl.h>
#include <imgui/imgui_impl_sdl.h>
#include <iostream>
#include <fstream>
#include <SDL_image.h>


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

	SDL_SetWindowResizable(window, SDL_TRUE);

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

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );

	GLfloat vertices[] =
	{
		// position				//color					//texture coordinate
		0.5f , 0.5f , 0.0f,	    1.0f, 0.0f,0.0f,		1.0f, 1.0f,
		0.5f , -0.5f , 0.0f,	1.0f, 1.0f,1.0f,		1.0f, 0.0f,
		-0.5f , -0.5f, 0.0f,	1.0f, 0.0f,0.0f,		0.0f, 0.0f,
		-0.5f , 0.5f, 0.0f,		1.0f, 0.0f,1.0f,		0.0f, 1.0f
	};

	GLuint indiees[] =
	{
		0,1,3, // firsth triangle
		1,2,3
	};

	//GLuint VBO, VAO , EBO;

	//must be after GLEW init
	shaderWithLight.loadShaders("/shaders/lighting_dir_point_spot_VERT.vert",
		"/shaders/lighting_dir_point_spot_GEOM.geom",
		"/shaders/lighting_dir_point_spot_FRAG.frag");
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indiees), indiees, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// texture coordinate attribute
	//				index amaunt type   , GL_FALSE, Array size         , start size
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // unbind

	//GLuint texture;
	int width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTextureParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		printf("IMG_Init: Failed to init required jpg and png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
		// handle error
	}
	


	// load sample.png into image
	SDL_Surface* image;
	image = IMG_Load("Sample.png");
	if (!image) {
		printf("IMG_Load: %s\n", IMG_GetError());
		// handle error
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	glGenerateMipmap( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, 0);
	

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
	glDeleteBuffers(1, &EBO);


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

	glm::mat4 model, view, projection;

	orbitCamera.rotate(orbitCamera.gYaw,orbitCamera.gPitch);
	orbitCamera.setRadius(orbitCamera.gRadius);

	// Create the View matrix
	view = orbitCamera.getViewMatrix();

	// Create the projection matrix
	projection = glm::perspective(glm::radians(orbitCamera.getFOV()), (float)windowWidth / (float)windowHeight, 1.0f, 200.0f);

	shaderWithLight.use();

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.5f, 0.5f, 0.0f));
	transform = glm::rotate(transform, (GLfloat)SDL_GetTicks() , glm::vec3(0.0f, 0.0f, 1.0f));
	
	GLint transformLocation = glGetUniformLocation(shader->shaderProgram, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));


	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader->shaderProgram, "ourTexture"), 0);

	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES,0,3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// draw OpenGL

	SDL_GL_SwapWindow(window);
}
