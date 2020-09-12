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

#include "RendererSDLOpenGL.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace ExoRenderer;
using namespace ExoRendererSDLOpenGL;

static RendererSDLOpenGL	*g_instance = nullptr;

RendererSDLOpenGL&	RendererSDLOpenGL::Get(void)
{
	if (!g_instance)
		g_instance = new RendererSDLOpenGL();
	return *g_instance;
}

void				 RendererSDLOpenGL::Destroy(void)
{
	if (g_instance)
	{
		delete g_instance;
		g_instance = nullptr;
	}
}

void RendererSDLOpenGL::initialize(const std::string& title, const int width, const int height, const WindowMode &mode, bool resizable)
{
	_scissorBit[0] = 0; _scissorBit[1] = 0; _scissorBit[2] = 0; _scissorBit[3] = 0;

	// Destroy if window already exist
	if (_pWindow)
		delete _pWindow;

	_pWindow = new Window(title, width, height, mode, resizable, _gamepad);
	resize();

	// Shaders
	loadShaders();

	// Buffers
	createBuffers();

	// Renderers
	_pObjectRenderer = new ObjectRenderer();
}

void RendererSDLOpenGL::resize()
{
	_perspective = glm::perspective(glm::radians(90.0f), (float)(_pWindow->getWidth() / _pWindow->getHeight()), 0.1f, 100.f);
	_orthographic = glm::ortho(0.0f, (float)_pWindow->getWidth(), (float)_pWindow->getHeight(), 0.0f, 0.0f, 1.0f);
}

// Create
ICamera* RendererSDLOpenGL::createCamera(void)
{
	return new Camera();
}

IAxis* RendererSDLOpenGL::createAxis(void)
{
	return new Axis();
}

ITexture* RendererSDLOpenGL::createTexture(const std::string& filePath, TextureFilter filter)
{
	ITexture	*texture;
	GLsync		fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		texture = new Texture(filePath, filter);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (texture);
	}
	else
		return (new Texture(filePath, filter));
}

ITexture* RendererSDLOpenGL::createTexture(unsigned int width, unsigned int height, TextureFormat format, TextureFilter filter)
{
	ITexture	*texture;
	GLsync		fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		texture = new Texture(width, height, format, filter);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (texture);
	}
	else
		return (new Texture(width, height, format, filter));
}

IArrayTexture* RendererSDLOpenGL::createArrayTexture(int width, int height, std::vector<std::string> &textures, TextureFilter filter)
{
	ArrayTexture	*texture;
	GLsync			fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		texture = new ArrayTexture(width, height, textures, filter);
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (new ArrayTexture(width, height, textures, filter));
	}
	else
		return (new ArrayTexture(width, height, textures, filter));
}

IFrameBuffer	*RendererSDLOpenGL::createFrameBuffer(void)
{
	IFrameBuffer	*frameBuffer;
	GLsync			fenceId;

	if (std::this_thread::get_id() != _mainThread)
	{
		_pWindow->handleThread();
		frameBuffer = new FrameBuffer();
		fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		while(glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
			;
		return (frameBuffer);
	}
	else
		return (new FrameBuffer());
}

// Push
void RendererSDLOpenGL::add(sprite &s)
{
	_pObjectRenderer->add(s);
}

// Push
void RendererSDLOpenGL::remove(sprite &s)
{
	_pObjectRenderer->remove(s);
}

void RendererSDLOpenGL::draw(void)
{
	// Renderers
	if (_pCurrentCamera)
	{
		if (_pMousePicker)
			_pMousePicker->update((IMouse*)&_mouse, _pWindow->getWidth(), _pWindow->getHeight(), ((Camera*)_pCurrentCamera)->getLookAt(), _perspective);

		_pObjectRenderer->render((Camera*)_pCurrentCamera, _perspective);

		if (_pAxis)
			((Axis*)_pAxis)->render(((Camera*)_pCurrentCamera)->getLookAt(), _perspective);
	}

	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	GL_CALL(glDisable(GL_BLEND));
}

void RendererSDLOpenGL::swap(void)
{
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	draw();
	GL_CALL(glDisable(GL_BLEND));

	_mouse.updateLastBuffer();
	_keyboard.updateLastBuffer();
	_gamepad.update();

	_pWindow->handleEvents(_keyboard, _mouse, _gamepad);

	_pWindow->swap();

	_pWindow->clearScreen();
}

// Getters
IWindow* RendererSDLOpenGL::getWindow(void)
{
	return _pWindow;
}

IKeyboard* RendererSDLOpenGL::getKeyboard(void)
{
	return &_keyboard;
}

IMouse* RendererSDLOpenGL::getMouse(void)
{
	return &_mouse;
}

IGamepadManager* RendererSDLOpenGL::getGamepadManager(void)
{
	return &_gamepad;
}

unsigned int RendererSDLOpenGL::getTime(void) const
{
	return SDL_GetTicks();
}

// Setters
void RendererSDLOpenGL::setMousePicker(MousePicker* picker)
{
	_pMousePicker = picker;
}

void RendererSDLOpenGL::setAxis(IAxis* axis)
{
	_pAxis = axis;
}

void RendererSDLOpenGL::setGridEnable(bool val)
{
	if (_pObjectRenderer)
		_pObjectRenderer->setGrid(val);
}

// Private
RendererSDLOpenGL::RendererSDLOpenGL(void)
: IRenderer(), _pWindow(nullptr), _pObjectRenderer(nullptr)
{
	_mainThread = std::this_thread::get_id();
}

RendererSDLOpenGL::~RendererSDLOpenGL(void)
{
	if (_pWindow)
		delete _pWindow;

	// Renderers
	if (_pObjectRenderer)
		delete _pObjectRenderer;

	// Shaders
	if (ObjectRenderer::pShader)
		delete ObjectRenderer::pShader;
}

void RendererSDLOpenGL::createBuffers(void)
{
	// SpriteRenderer
	const float vertexBuffer[] = {
		-0.5f,	0.5f, 0.0f,	// top left
		-0.5f, -0.5f, 0.0f,	// bottom left
		 0.5f, -0.5f, 0.0f,	// bottom right
		 0.5f, 0.5f, 0.0f,	// top right
	};

	const unsigned int indexBuffer[] = {
		0, 1, 3,
		1, 2, 3
	};
	const float UVBuffer[] = {
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	ObjectRenderer::vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	ObjectRenderer::vertexBuffer = new Buffer(12, 3, &vertexBuffer, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 0, false);
	ObjectRenderer::indexBuffer = new Buffer(6, 3, &indexBuffer, BufferType::INDEXBUFFER, BufferDraw::STATIC, 0, false);
	ObjectRenderer::uvBuffer = new Buffer(8, 2, &UVBuffer, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 1, true);

	// Grid
	const float line[] = {
		0.0,	0.0,
		1.0,	0.0,
	};

	Grid::vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	Grid::vertexBuffer = new Buffer(4, 2, &line, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 0, false);

	// Axis
	const float triangleVertexBuffer[] = {
		 0.0f,	0.15f, 0.0f,	// top
		-0.05f, -0.05f, 0.0f,	// bottom left
		 0.05f, -0.05f, 0.0f	// bottom right
	};

	Axis::vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
	Axis::vertexBuffer = new Buffer(9, 3, &triangleVertexBuffer, BufferType::ARRAYBUFFER, BufferDraw::STATIC, 0, false);
}

#ifndef USE_TEST_SHADERS

static const std::vector<std::string>	g_2DShader = {
	"#version 330 core",
	"",
	"layout(location = 0) in vec3 position;",
	"layout(location = 1) in vec2 texCoord;",
	"",
	"uniform mat4 view;",
	"uniform mat4 projection;",
	"uniform mat4 model;",
	"",
	"out vec2 TexCoords;",
	"",
	"void main(void) ",
	"{",
	"    gl_Position = projection * view * model * vec4(position, 1.0);",
	"    TexCoords = texCoord;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330 core",
	"",
	"in vec2 TexCoords;",
	"",
	"uniform sampler2DArray ourTexture;",
	"uniform int layer;",
	"uniform int flipHorizontal;",
	"uniform int flipVertical;",
	"uniform float size;",
	"",
	"out vec4 color;",
	"",
	"void main(void) ",
	"{    ",
	"    vec4 color_out = texture(ourTexture, vec3(TexCoords.x * size * flipHorizontal, TexCoords.y * size * flipVertical, layer));",
	"",
	"    if(color_out.a < 0.1)",
	"        discard;",
	"",
	"	color = color_out;",
	"}"
};

static const std::vector<std::string>	g_guiShader = {
	"#version 330 core",
	"layout (location = 0) in vec2 position;",
	"",
	"uniform mat4 transformation;",
	"uniform mat4 projection;",
	"",
	"out vec2 TexCoords;",
	"",
	"void main(void)",
	"{",
	"    gl_Position = projection * transformation * vec4(position, 0.0, 1.0);",
	"    TexCoords = vec2((position.x + 1.0) / 2, 1 - (-1 * position.y + 1.0) / 2.0);",
	"}",
	"",
	"#FRAGMENT",
	"#version 330 core",
	"",
	"in vec2 TexCoords;",
	"",
	"out vec4 color;",
	"",
	"uniform sampler2D guiTexture;",
	"uniform float opacity;",
	"uniform float numberOfRows;",
	"uniform float numberOfColumns;",
	"uniform vec2 offset;",
	"",
	"void main(void)",
	"{    ",
	"    color = texture(guiTexture, vec2(TexCoords.x / numberOfRows, TexCoords.y / numberOfColumns) + offset);",
	"    color.a = color.a - opacity;",
	"}"
};

static const std::vector<std::string>	g_fontShader = {
	"#version 330 core",
	"layout (location = 0) in vec4 vertex;",
	"",
	"uniform mat4 projection;",
	"",
	"out vec2 TexCoords;",
	"",
	"void main()",
	"{",
	"    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);",
	"    TexCoords = vertex.zw;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330 core",
	"",
	"in vec2 TexCoords;",
	"out vec4 color;",
	"",
	"uniform sampler2D fontAtlas;",
	"uniform vec3 textColor;",
	"",
	"void main()",
	"{    ",
	"    color = vec4(textColor, texture(fontAtlas, TexCoords).r);",
	"}"
};

static const std::vector<std::string>	g_lineShader = {
	"#version 330",
	"",
	"layout(location = 0) in vec3 position;",
	"",
	"uniform mat4 view;",
	"uniform mat4 projection;",
	"uniform mat4 model;",
	"uniform vec4 color;",
	"",
	"out vec4 in_color;",
	"",
	"void main(void)",
	"{",
	"  gl_Position = projection * view * model * vec4(position, 1.0);",
	"  in_color = color;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330",
	"",
	"in vec4 in_color;",
	"out vec4 color;",
	"",
	"void main(void)",
	"{",
	"  color = in_color;",
	"}"
};

static const std::vector<std::string>	g_axisShader = {
	"#version 330",
	"",
	"layout(location = 0) in vec3 position;",
	"",
	"uniform mat4 view;",
	"uniform mat4 projection;",
	"uniform mat4 model;",
	"uniform vec4 color;",
	"",
	"out vec4 in_color;",
	"",
	"void main(void)",
	"{",
	"  gl_Position = projection * view * model * vec4(position, 1.0);",
	"  in_color = color;",
	"}",
	"",
	"#FRAGMENT",
	"#version 330",
	"",
	"in vec4 in_color;",
	"out vec4 color;",
	"",
	"void main(void)",
	"{",
	"  color = in_color;",
	"}"
};

#endif

void RendererSDLOpenGL::loadShaders(void)
{
#ifdef USE_TEST_SHADERS
	ObjectRenderer::pShader = new Shader("resources/shaders/OpenGL3/2D.glsl");
	GUIRenderer::pGuiShader = new Shader("resources/shaders/OpenGL3/gui.glsl");
	TextRenderer::pTextShader = new Shader("resources/shaders/OpenGL3/font.glsl");
	Grid::pShader = new Shader("resources/shaders/OpenGL3/line.glsl");
	Axis::pShader = new Shader("resources/shaders/OpenGL3/axis.glsl");
#else
	ObjectRenderer::pShader = new Shader(g_2DShader);
	Grid::pShader = new Shader(g_lineShader);
	Axis::pShader = new Shader(g_axisShader);
#endif
}
