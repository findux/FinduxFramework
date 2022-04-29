#pragma once
#include<GL/glew.h>
#include <SDL.h>
#include "Shader.h"

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class App
{
public:
	bool isRunning;
	bool isDebug;
	int millisecsPreviousFrame = 0;
	static int windowWidth;
	static int windowHeight;
	Shader* shader;
public:
	App();
	~App();
	void initialize();
	void run();
	void destroy();

	void setup();
	void processInput();
	void update();
	void render();
private:
	SDL_Window* window;
	SDL_GLContext mainContext;
};

