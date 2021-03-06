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

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Grid.h"
#include "Shader.h"
#include "Buffer.h"

namespace ExoEngine
{

	enum class AxisType {
		TRANSLATION,
		SCALE
	};

	class Axis
	{
	public:
		Axis(void);
		virtual ~Axis(void);

		void render(const glm::mat4& lookAt, const glm::mat4& perspective);

		// Getters
		const AxisType& getType(void) const { return _type; }
		const glm::vec2& getPosition(void) const { return _pos; }

		// Setters
		void setType(const AxisType& type) { _type = type; };
		void setPosition(const glm::vec2& position) { _pos = position; }
	private:
		void drawAxis(int x, int y, float angle, const glm::vec3 &color, const glm::mat4& lookAt, const glm::mat4& perspective);
	public:
		static Shader* pShader;

		// Triangle
		static Buffer* vaoBuffer;
		static Buffer* vertexBuffer;
	private:
		AxisType _type;
		glm::vec2 _pos;
	};

}
