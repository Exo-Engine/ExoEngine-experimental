/*
 *	MIT License
 *
 *	Copyright (c) 2020 Gaëtan Dezeiraud and Ribault Paul
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 */

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include <Engine.h>
// #include <SDL2/SDL.h>
// #include <Window.h>

using namespace	ExoEngine;

void setupImGui(IWindow* window)
{
	// Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Style
	ImGui::StyleColorsDark();

	// Setup OpenGL
	ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)window->getWindowID(), window->getGLContext());
	ImGui_ImplOpenGL3_Init("#version 130");
}

void cleanImGui(void)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void renderImGui(IWindow* window)
{
	// ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame((SDL_Window*)window->getWindowID());
	ImGui::NewFrame();

	{
		ImGui::Begin("Hello, world!");
		ImGui::Text("This is some useful text.");
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int	main(void)
{
	Engine		engine("resources/settings.xml");
	IRenderer*	renderer = engine.getRenderer();
	IWindow*	window;
	Mouse*      mouse;
	Keyboard*	keyboard;
	bool		run = true;

	renderer->initialize("Editor", 1280, 720, WindowMode::WINDOWED, true);
	engine.getResourceManager()->load("resources/resources.xml");
	window = renderer->getWindow();
	mouse = renderer->getMouse();
	keyboard = renderer->getKeyboard();

	window->setVsync(true);

	setupImGui(window);

	auto cam = renderer->createCamera();
	cam->setPosition(0.0f, 0.0f, 20.0f);

	renderer->setCurrentCamera(cam);
	renderer->setGridEnable(true);

	sprite img;
	img.texture = engine.getResourceManager()->get<IArrayTexture>("cursor");
	img.scale = glm::vec2(3.6);

	renderer->add(img);

	while (run)
	{
		if (keyboard->isKeyDown(KeyboardKeys::KEY_ESCAPE))
			run = false;

		if (window->getIsClosing())
			run = false;

		cam->update(mouse, keyboard);

		// ImGui_ImplSDL2_ProcessEvent(&event);
		renderImGui(window);
		renderer->swap();
	}

	cleanImGui();
	return (EXIT_SUCCESS);
}
