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

namespace ExoEngine {

	void RendererSDLOpenGL::initialize(const std::string& title, const int width, const int height, const WindowMode& mode, bool resizable)
	{
		_scissorBit[0] = 0; _scissorBit[1] = 0; _scissorBit[2] = 0; _scissorBit[3] = 0;

		// Destroy if window already exist
		if (_pWindow)
			delete _pWindow;

		_pWindow = new Window(title, width, height, mode, resizable);
		resize();

		// Shaders
		loadShaders();

		// Buffers
		createBuffers();

		// Renderers
		_pObjectRenderer = new ObjectRenderer();
		_pGUIRenderer = new GUIRenderer();
		_pTextRenderer = new TextRenderer();
	}

	void RendererSDLOpenGL::resize()
	{
		_UIScaleFactor = _pWindow->getWidth() / REFRENCE_RESOLUTION_WIDTH;

		_perspective = glm::perspective(glm::radians(90.0f), (float)(_pWindow->getWidth() / _pWindow->getHeight()), 0.1f, 100.f);
		_orthographic = glm::ortho(0.0f, (float)_pWindow->getWidth(), (float)_pWindow->getHeight(), 0.0f, 0.0f, 1.0f);
	}

	// Create
	ICamera* RendererSDLOpenGL::createCamera(void)
	{
		return new Camera();
	}

	ITexture* RendererSDLOpenGL::createTexture(const std::string& filePath, TextureFilter filter)
	{
		ITexture* texture;
		GLsync		fenceId;

		if (std::this_thread::get_id() != _mainThread)
		{
			_pWindow->handleThread();
			texture = new Texture(filePath, filter);
			fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
			while (glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
				;
			return (texture);
		}
		else
			return (new Texture(filePath, filter));
	}

	ITexture* RendererSDLOpenGL::createTexture(unsigned int width, unsigned int height, TextureFormat format, TextureFilter filter)
	{
		ITexture* texture;
		GLsync		fenceId;

		if (std::this_thread::get_id() != _mainThread)
		{
			_pWindow->handleThread();
			texture = new Texture(width, height, format, filter);
			fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
			while (glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
				;
			return (texture);
		}
		else
			return (new Texture(width, height, format, filter));
	}

	IArrayTexture* RendererSDLOpenGL::createArrayTexture(int width, int height, std::vector<std::string>& textures, TextureFilter filter)
	{
		ArrayTexture* texture;
		GLsync			fenceId;

		if (std::this_thread::get_id() != _mainThread)
		{
			_pWindow->handleThread();
			texture = new ArrayTexture(width, height, textures, filter);
			fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
			while (glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
				;
			return (new ArrayTexture(width, height, textures, filter));
		}
		else
			return (new ArrayTexture(width, height, textures, filter));
	}

	IImage* RendererSDLOpenGL::createImage(const std::shared_ptr<ITexture>& texture)
	{
		return new Image(texture, _UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
	}

	IFrameBuffer* RendererSDLOpenGL::createFrameBuffer(void)
	{
		IFrameBuffer* frameBuffer;
		GLsync			fenceId;

		if (std::this_thread::get_id() != _mainThread)
		{
			_pWindow->handleThread();
			frameBuffer = new FrameBuffer();
			fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
			while (glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(1000000)) == GL_TIMEOUT_EXPIRED)
				;
			return (frameBuffer);
		}
		else
			return (new FrameBuffer());
	}

	// Push
	void RendererSDLOpenGL::add(sprite& s)
	{
		_pObjectRenderer->add(s);
	}

	void RendererSDLOpenGL::add(IWidget* widget)
	{
		widget->update(getMouse(), getKeyboard(), getNavigationType());

		// Update
		switch (widget->getType())
		{
			case IWidget::BUTTON: {
				auto button = (Button*)widget;
				if (button->getLabel())
					add(button->getLabel());
				break;
			}
			case IWidget::SELECT: {
				auto select = (Select*)widget;
				add(select->getLabel());
				break;
			}
			case IWidget::INPUT: {
				auto input = (Input*)widget;
				add(input->getLabel());
				break;
			}
			case IWidget::SPINNER: {
				auto spinner = (Spinner*)widget;
				spinner->update(_pWindow->getDelta());
				break;
			}
			default: break;
		}

		// Push in renderer
		_pGUIRenderer->add(widget);
	}

	void RendererSDLOpenGL::add(Label* label)
	{
		label->contextInfo(_UIScaleFactor, _pWindow->getWidth(), _pWindow->getHeight());
		_pTextRenderer->add(label);
	}

	void RendererSDLOpenGL::remove(sprite& s)
	{
		_pObjectRenderer->remove(s);
	}

	void RendererSDLOpenGL::remove(IWidget* widget)
	{
		// Update
		switch (widget->getType())
		{
			case IWidget::BUTTON: {
				auto button = (Button*)widget;
				remove(button->getLabel());
				break;
			}
			case IWidget::SELECT: {
				auto select = (Select*)widget;
				remove(select->getLabel());
				break;
			}
			case IWidget::INPUT: {
				auto input = (Input*)widget;
				remove(input->getLabel());
				break;
			}
			default: break;
		}
		// Push in renderer
		_pGUIRenderer->remove(widget);
	}

	void RendererSDLOpenGL::remove(Label* label)
	{
		_pTextRenderer->remove(label);
	}

	void RendererSDLOpenGL::draw(void)
	{		
		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Renderers
		if (_pCurrentCamera)
		{
			if (_pMousePicker)
				_pMousePicker->update((Mouse*)&_mouse, _pWindow->getWidth(), _pWindow->getHeight(), ((Camera*)_pCurrentCamera)->getLookAt(), _perspective);

			_pObjectRenderer->render((Camera*)_pCurrentCamera, _perspective);

			if (_pAxis)
				((Axis*)_pAxis)->render(((Camera*)_pCurrentCamera)->getLookAt(), _perspective);
		}

		_pGUIRenderer->render(_orthographic);
		_pTextRenderer->render(_orthographic);

		GL_CALL(glDisable(GL_BLEND));
	}

	void RendererSDLOpenGL::swap(void)
	{
		_mouse.updateLastBuffer();
		_keyboard.updateLastBuffer();

		_pWindow->handleEvents(_keyboard, _mouse);
		if (_pCursor)
			_pCursor->update();

		_pWindow->swap();
		_pWindow->clearScreen();
	}

	void RendererSDLOpenGL::beginScissor(glm::vec2 position, glm::vec2 size, glm::vec2 parentPosition, glm::vec2 parentSize)
	{
		position *= (!_pWindow->isFullscreen() ? _pWindow->getHighDPIFactor() : 1);
		size *= (!_pWindow->isFullscreen() ? _pWindow->getHighDPIFactor() : 1);
		parentPosition *= (!_pWindow->isFullscreen() ? _pWindow->getHighDPIFactor() : 1);
		parentSize *= (!_pWindow->isFullscreen() ? _pWindow->getHighDPIFactor() : 1);

		if (parentPosition.x != 0 && parentPosition.y != 0 && parentSize.x != 0 && parentSize.y != 0)
		{
			glGetIntegerv(GL_SCISSOR_BOX, _scissorBit);
			// X
			if (position.x + size.x > parentPosition.x + parentSize.x) // Right
				size.x = (parentPosition.x + parentSize.x) - position.x;

			if (position.x < parentPosition.x) // Left
			{
				size.x = (position.x + size.x) - parentPosition.x;
				position.x = parentPosition.x;
			}

			// Y
			if (position.y + size.y > parentPosition.y + parentSize.y) // Top
				size.y = (parentPosition.y + parentSize.y) - position.y;

			if (position.y < parentPosition.y) // Bottom
			{
				size.y = (position.y + size.y) - parentPosition.y;
				position.y = parentPosition.y;
			}
		}

		glEnable(GL_SCISSOR_TEST);
		glScissor(position.x, position.y, size.x, size.y);
	}

	void RendererSDLOpenGL::endScissor(void)
	{
		glScissor(_scissorBit[0], _scissorBit[1], _scissorBit[2], _scissorBit[3]);
		glDisable(GL_SCISSOR_TEST);

		// Reset
		_scissorBit[0] = 0; _scissorBit[1] = 0; _scissorBit[2] = 0; _scissorBit[3] = 0;
	}

	// Getters
	IWindow* RendererSDLOpenGL::getWindow(void)
	{
		return _pWindow;
	}

	Keyboard* RendererSDLOpenGL::getKeyboard(void)
	{
		return &_keyboard;
	}

	Mouse* RendererSDLOpenGL::getMouse(void)
	{
		return &_mouse;
	}

	unsigned int RendererSDLOpenGL::getTime(void) const
	{
		return SDL_GetTicks();
	}

	void RendererSDLOpenGL::setCursor(ICursor* cursor)
	{
		if (_pCursor)
			remove(_pCursor->getImage());

		_pCursor = (Cursor*)cursor;
		if (_pCursor)
			add(_pCursor->getImage());
	}

	// Setters
	void RendererSDLOpenGL::setMousePicker(MousePicker* picker)
	{
		_pMousePicker = picker;
	}

	void RendererSDLOpenGL::setAxis(Axis* axis)
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
		: IRenderer(), _pWindow(nullptr), _pObjectRenderer(nullptr), _pGUIRenderer(nullptr), _pTextRenderer(nullptr), _pCursor(nullptr)
	{
		_mainThread = std::this_thread::get_id();
	}

	RendererSDLOpenGL::~RendererSDLOpenGL(void)
	{
		if (_pWindow)
			delete _pWindow;

		if (_pCursor)
			delete _pCursor;

		// Renderers
		if (_pObjectRenderer)
			delete _pObjectRenderer;

		if (_pGUIRenderer)
			delete _pGUIRenderer;

		if (_pTextRenderer)
			delete _pTextRenderer;

		// Buffers
		if (TextRenderer::vaoBuffer)
			delete TextRenderer::vaoBuffer;

		if (TextRenderer::vertexBuffer)
			delete TextRenderer::vertexBuffer;

		// Shaders
		if (ObjectRenderer::pShader)
			delete ObjectRenderer::pShader;

		if (GUIRenderer::pGuiShader)
			delete GUIRenderer::pGuiShader;

		if (TextRenderer::pTextShader)
			delete TextRenderer::pTextShader;
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

		// TextRenderer
		TextRenderer::vaoBuffer = new Buffer(0, 0, NULL, BufferType::VERTEXARRAY, BufferDraw::STATIC, 0, false);
		TextRenderer::vertexBuffer = new Buffer(24, 4, NULL, BufferType::ARRAYBUFFER, BufferDraw::DYNAMIC, 0, false);

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

	static const std::vector<std::string> g_2DShader = {
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

	static const std::vector<std::string> g_guiShader = {
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

	static const std::vector<std::string> g_lineShader = {
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

	static const std::vector<std::string> g_axisShader = {
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
		GUIRenderer::pGuiShader = new Shader(g_guiShader);
		TextRenderer::pTextShader = new Shader(g_fontShader);
		Grid::pShader = new Shader(g_lineShader);
		Axis::pShader = new Shader(g_axisShader);
#endif
	}

}