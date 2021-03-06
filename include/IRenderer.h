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

#include "Enums.h"
#include "IWindow.h"
#include "Keyboard.h"
#include "ICamera.h"
#include "Mouse.h"
#include "IShader.h"
#include "ITexture.h"
#include "IArrayTexture.h"
#include "IFrameBuffer.h"
#include "sprite.h"
#include "MousePicker.h"
#include "Axis.h"
#include "UI/ICursor.h"
#include "UI/Label.h"

namespace ExoEngine
{

	class IRenderer
	{
	public:
		IRenderer(void): 
			_currentNavigationType(NavigationType::MOUSE),
			_pCurrentCamera(nullptr), 
			_pMousePicker(nullptr), 
			_pAxis(nullptr)
		{ };

		virtual ~IRenderer(void)
		{ };

		virtual void initialize(const std::string& title, const int width, const int height, const WindowMode &mode, bool resizable) = 0;
		virtual void resize() = 0;

		virtual ICamera		    *createCamera(void) = 0;
		virtual ITexture		*createTexture(const std::string& filePath, TextureFilter filter = TextureFilter::LINEAR) = 0;
		virtual ITexture		*createTexture(unsigned int width, unsigned int height, TextureFormat format = TextureFormat::RGBA, TextureFilter filter = TextureFilter::LINEAR) = 0;
		virtual IArrayTexture	*createArrayTexture(int width, int height, std::vector<std::string> &textures, TextureFilter filter = TextureFilter::LINEAR) = 0;
		virtual IFrameBuffer	*createFrameBuffer(void) = 0;

		virtual IImage* createImage(const std::shared_ptr<ITexture>& texture) = 0;

		virtual void add(sprite &s) = 0;
		virtual void add(IWidget* widget) = 0;
		virtual void add(Label* label) = 0;

		virtual void remove(sprite &s) = 0;
		virtual void remove(IWidget* widget) = 0;
		virtual void remove(Label* label) = 0;

		virtual void draw(void) = 0;
		virtual void swap(void) = 0;

		// Getters
		NavigationType& getNavigationType(void) { return _currentNavigationType; }
		float getUIScaleFactor(void) { return _UIScaleFactor; }

		ICamera *getCurrentCamera(void) { return _pCurrentCamera; }
		virtual IWindow *getWindow(void) = 0;

		virtual Keyboard *getKeyboard(void) = 0;
		virtual Mouse *getMouse(void) = 0;
		virtual unsigned int getTime(void) const = 0;

		// Setters
		void setNavigationType(const NavigationType& type) { _currentNavigationType = type; }
		void setCurrentCamera(ICamera* camera) { _pCurrentCamera = camera; }
		virtual void setCursor(ICursor* cursor) = 0;
		virtual void setMousePicker(MousePicker* picker) = 0;
		virtual void setAxis(Axis* axis) = 0;
		virtual void setGridEnable(bool val) = 0;
	protected:
		NavigationType _currentNavigationType;
		float		_UIScaleFactor;
		ICamera	 *_pCurrentCamera;
		MousePicker *_pMousePicker;
		Axis		*_pAxis;
	};

}
