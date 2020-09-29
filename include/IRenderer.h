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
#include "IKeyboard.h"
#include "ICamera.h"
#include "IGamepadManager.h"
#include "IMouse.h"
#include "IShader.h"
#include "ITexture.h"
#include "IArrayTexture.h"
#include "IFrameBuffer.h"
#include "sprite.h"
#include "MousePicker.h"
#include "IAxis.h"

namespace ExoEngine
{

	class IRenderer
	{
	public:
		IRenderer(void): _pCurrentCamera(nullptr), _pMousePicker(nullptr), _pAxis(nullptr)
		{ };

		virtual ~IRenderer(void)
		{ };

		virtual void initialize(const std::string& title, const int width, const int height, const WindowMode &mode, bool resizable) = 0;
		virtual void resize() = 0;

		virtual ICamera		    *createCamera(void) = 0;
		virtual IAxis			*createAxis(void) = 0;
		virtual ITexture		*createTexture(const std::string& filePath, TextureFilter filter = TextureFilter::LINEAR) = 0;
		virtual ITexture		*createTexture(unsigned int width, unsigned int height, TextureFormat format = TextureFormat::RGBA, TextureFilter filter = TextureFilter::LINEAR) = 0;
		virtual IArrayTexture	*createArrayTexture(int width, int height, std::vector<std::string> &textures, TextureFilter filter = TextureFilter::LINEAR) = 0;
		virtual IFrameBuffer	*createFrameBuffer(void) = 0;

		virtual void add(sprite &s) = 0;
		virtual void remove(sprite &s) = 0;

		virtual void swap(void) = 0;

		// Getters
		ICamera *getCurrentCamera(void) { return _pCurrentCamera; }
		virtual IWindow *getWindow(void) = 0;

		virtual IKeyboard *getKeyboard(void) = 0;
		virtual IMouse *getMouse(void) = 0;
		virtual IGamepadManager *getGamepadManager(void) = 0;
		virtual unsigned int getTime(void) const = 0;

		// Setters
		void setCurrentCamera(ICamera* camera) { _pCurrentCamera = camera; }
		virtual void setMousePicker(MousePicker* picker) = 0;
		virtual void setAxis(IAxis* axis) = 0;
		virtual void setGridEnable(bool val) = 0;
	protected:
		virtual void draw(void) = 0;
	protected:
		ICamera	 *_pCurrentCamera;
		MousePicker *_pMousePicker;
		IAxis		*_pAxis;
	};

}
