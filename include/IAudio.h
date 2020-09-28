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

#include <vector>
#include <string>
#include <glm/vec3.hpp>

#include "ISource.h"
#include "ISound.h"

namespace	ExoAudio
{

class IAudio
{
public:
	IAudio(void);
	virtual ~IAudio(void);

	virtual void initialize(void) = 0;

	virtual void getDevices(std::vector<std::string> &devices) = 0;
	virtual void updateListener(const glm::vec3 &position, const glm::vec3 &velocity, float volume) = 0;
	virtual void updateVolume(float volume) = 0;

	virtual ISource* createSource(void) = 0;
	virtual ISound* createSound(const std::string& filePath) = 0;
	virtual IMusic* createMusic(const std::string& filePath) = 0;
};

}
