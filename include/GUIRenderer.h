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

#include <deque>
#include <functional>

#include "Shader.h"
#include "Buffer.h"

#include "TextRenderer.h"
#include "UI/IWidget.h"
#include "UI/Button.h"
#include "UI/Checkbox.h"
#include "UI/Select.h"
#include "UI/Input.h"
#include "UI/Slider.h"
#include "UI/Image.h"
#include "UI/Spinner.h"
#include "UI/View.h"

namespace ExoEngine
{

	class GUIRenderer
	{
	public:
		GUIRenderer(void);
		~GUIRenderer(void);

		void add(IWidget* widget);
		void remove(IWidget* widget);
		void render(const glm::mat4& orthographic);
	private:
		void prepare(const glm::mat4& orthographic);
	private:
		unsigned char render(IWidget* widget, Shader* shader);
		unsigned char render(Button* button, Shader* shader);
		unsigned char render(Checkbox* checkbox, Shader* shader);
		unsigned char render(Select* select, Shader* shader);
		unsigned char render(Input* input, Shader* shader);
		unsigned char render(Image* image, Shader* shader);
		unsigned char render(Spinner* spinner, Shader* shader);
		unsigned char render(View* view, Shader* shader);
		unsigned char render(Slider* slider, Shader* shader);

		static void drawSliced(Shader* shader, unsigned int offsetX, unsigned int offsetY, float positionX, float positionY, float sizeX, float sizeY, bool isHoverOffset, unsigned int numberOfRows, unsigned int numberOfColumns);
	public:
		static Shader* pGuiShader;
	private:
		std::deque<IWidget*> _renderQueue;
		std::deque<IWidget*> _renderFrontQueue;

		glm::mat4 _orthographic;
		Buffer* _vaoBuffer;
		Buffer* _vertexBuffer;
	};	

}
