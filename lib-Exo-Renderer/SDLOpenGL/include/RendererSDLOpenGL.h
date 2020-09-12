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

#pragma once

#include <string>
#include <thread>
#include <glm/mat4x4.hpp>

#include "IRenderer.h"

#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "GamepadManager.h"
#include "Gamepad.h"
#include "ObjectRenderer.h"
#include "Grid.h"
#include "Shader.h"
#include "Texture.h"
#include "ArrayTexture.h"

#include <vector>

namespace ExoRendererSDLOpenGL
{

class RendererSDLOpenGL : public ExoRenderer::IRenderer
{
public:

	//	Singleton methods
	static RendererSDLOpenGL&	Get(void);
	static void				 Destroy(void);

	//	class methods
	virtual void initialize(const std::string& title, const int width, const int height, const ExoRenderer::WindowMode &mode, bool resizable);
	virtual void resize();

	virtual ExoRenderer::ICamera		*createCamera(void);
	virtual ExoRenderer::IAxis			*createAxis(void);
	virtual ExoRenderer::ITexture		*createTexture(const std::string& filePath, ExoRenderer::TextureFilter filter = ExoRenderer::TextureFilter::LINEAR);
	virtual ExoRenderer::ITexture		*createTexture(unsigned int width, unsigned int height, ExoRenderer::TextureFormat format = ExoRenderer::TextureFormat::RGBA, ExoRenderer::TextureFilter filter = ExoRenderer::TextureFilter::LINEAR);
	virtual ExoRenderer::IArrayTexture	*createArrayTexture(int width, int height, std::vector<std::string> &textures, ExoRenderer::TextureFilter filter = ExoRenderer::TextureFilter::LINEAR);
	virtual ExoRenderer::IFrameBuffer	*createFrameBuffer(void);

	virtual void add(ExoRenderer::sprite &s);
	virtual void remove(ExoRenderer::sprite &s);

	virtual void swap(void);

	// Getters
	virtual ExoRenderer::IWindow *getWindow(void);

	virtual ExoRenderer::IKeyboard *getKeyboard(void);
	virtual ExoRenderer::IMouse *getMouse(void);
	virtual ExoRenderer::IGamepadManager *getGamepadManager(void);
	virtual unsigned int getTime(void) const;

	// Setters
	virtual void setMousePicker(ExoRenderer::MousePicker* picker);
	virtual void setAxis(ExoRenderer::IAxis* axis);
	virtual void setGridEnable(bool val);
private:
	RendererSDLOpenGL(void);
	virtual ~RendererSDLOpenGL(void);

	void createBuffers(void);
	void loadShaders(void);

	virtual void draw(void);
private:
	Window* _pWindow;

	Keyboard _keyboard;
	Mouse _mouse;
	GamepadManager _gamepad;

	ObjectRenderer* _pObjectRenderer;

	glm::mat4 _perspective, _orthographic;
	int _scissorBit[4];

	std::thread::id _mainThread;
};

}
