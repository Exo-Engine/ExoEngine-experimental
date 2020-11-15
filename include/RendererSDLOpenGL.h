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
#include "ObjectRenderer.h"
#include "GUIRenderer.h"
#include "Grid.h"
#include "Shader.h"
#include "Texture.h"
#include "ArrayTexture.h"

#include <vector>
#include <UI/Cursor.h>

#include "Utils/Singleton.h"

namespace ExoEngine
{

	class RendererSDLOpenGL : public IRenderer, public Singleton<RendererSDLOpenGL>
	{
	public:

		//	class methods
		virtual void initialize(const std::string& title, const int width, const int height, const WindowMode &mode, bool resizable);
		virtual void resize();

		virtual ICamera		*createCamera(void);
		virtual ITexture		*createTexture(const std::string& filePath, TextureFilter filter = TextureFilter::LINEAR);
		virtual ITexture		*createTexture(unsigned int width, unsigned int height, TextureFormat format = TextureFormat::RGBA, TextureFilter filter = TextureFilter::LINEAR);
		virtual IArrayTexture	*createArrayTexture(int width, int height, std::vector<std::string> &textures, TextureFilter filter = TextureFilter::LINEAR);

		virtual IImage* createImage(const std::shared_ptr<ITexture>& texture);

		/*virtual ILabel* createLabel();
		virtual IButton* createButton(const std::shared_ptr<ITexture>& texture, ButtonType buttonType = ButtonType::NORMAL, bool withLabel = true);
		virtual ICheckbox* createCheckbox(const std::shared_ptr<ITexture>& texture, bool checked = false);
		virtual IInput* createInput(const std::shared_ptr<ITexture>& texture, const std::string& text = "", InputType type = InputType::TEXT);
		virtual ISpinner* createSpinner(const std::shared_ptr<ITexture>& texture);
		virtual ISlider* createSlider(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& barTexture);
		virtual ISelect* createSelect(const std::shared_ptr<ITexture>& buttonTexture, const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, const std::shared_ptr<Font>& font);
		virtual IView* createView(const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows = 1, unsigned int numberOfColumns = 1);
		virtual IView* createView(const std::shared_ptr<ITexture>& backgroundTexture, const std::shared_ptr<ITexture>& scrollTexture, unsigned int numberOfRows = 1, unsigned int numberOfColumns = 1);*/
		virtual IFrameBuffer* createFrameBuffer(void);

		virtual void add(sprite &s);
		virtual void add(IWidget* widget);
		virtual void add(ILabel* label);

		virtual void remove(sprite &s);
		virtual void remove(IWidget *widget);
		virtual void remove(ILabel *label);

		virtual void draw(void);
		virtual void swap(void);

		void beginScissor(glm::vec2 position, glm::vec2 size, glm::vec2 parentPosition, glm::vec2 parentSize);
		void endScissor(void);

		// Getters
		virtual IWindow *getWindow(void);

		virtual Keyboard *getKeyboard(void);
		virtual Mouse *getMouse(void);
		virtual unsigned int getTime(void) const;

		// Setters
		virtual void setCursor(ICursor* cursor);
		virtual void setMousePicker(MousePicker* picker);
		virtual void setAxis(Axis* axis);
		virtual void setGridEnable(bool val);
	private:
		RendererSDLOpenGL(void);
		virtual ~RendererSDLOpenGL(void);

		void createBuffers(void);
		void loadShaders(void);
	private:
		friend class Singleton<RendererSDLOpenGL>;
		Window* _pWindow;

		Keyboard _keyboard;
		Mouse _mouse;

		ObjectRenderer* _pObjectRenderer;
		GUIRenderer* _pGUIRenderer;
		// TextRenderer* _pTextRenderer;

		glm::mat4 _perspective, _orthographic;
		int _scissorBit[4];

		std::thread::id _mainThread;
		Cursor* _pCursor;
	};

}
