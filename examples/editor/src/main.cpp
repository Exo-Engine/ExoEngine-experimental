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
#include "ImGui/imgui_internal.h"

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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

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

	// Dockspace
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::SetNextWindowBgAlpha(0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiDockNodeFlags_PassthruCentralNode;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", (bool*)true, window_flags);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	if (ImGui::DockBuilderGetNode(ImGui::GetID("Dockspace")) == NULL)
	{
		ImGuiID dockspace_id = ImGui::GetID("Dockspace");
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
		ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_PassthruCentralNode); // Add empty node

		ImGuiID dock_main_id = dockspace_id;
		ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
		ImGuiID dock_id_left_bottom = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.20f, NULL, &dock_id_left);
		ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
		ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.20f, NULL, &dock_main_id);

		ImGui::DockBuilderDockWindow("Project", dock_id_left);
		ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left_bottom);
		ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
		ImGui::DockBuilderFinish(dockspace_id);
	}

	// Menu Bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Project"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Windows
	ImGuiID dockspace_id = ImGui::GetID("Dockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

	ImGui::Begin("Project");
	ImGui::Text("Text 1");
	ImGui::End();

	ImGui::Begin("Hierarchy");
	ImGui::Text("Text 2");
	ImGui::End();

	ImGui::Begin("Inspector");
	ImGui::Text("Text 3");
	ImGui::End();

	ImGui::End();

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

	renderer->initialize("ExoEngine - Editor", 1280, 720, WindowMode::WINDOWED, true);
	engine.getResourceManager()->load("resources/resources.xml");
	window = renderer->getWindow();
	mouse = renderer->getMouse();
	keyboard = renderer->getKeyboard();

	window->isCursorVisible(true);
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

		renderer->draw();
		renderImGui(window);

		renderer->swap();
	}

	cleanImGui();
	return (EXIT_SUCCESS);
}
