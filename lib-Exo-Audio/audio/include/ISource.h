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

#include <glm/vec3.hpp>

#include "ISound.h"
#include "IMusic.h"

namespace	ExoAudio
{

class ISource
{
public:
	enum SourceState {
		INITIAL,
		STOPPED,
		PLAYING,
		PAUSED
	};
public:
	ISource(void);
	virtual ~ISource(void);

	virtual void play(void) const = 0;
	virtual void stop(void) const = 0;
	virtual void rewind(void) const = 0;

	virtual void streamingUpdate(void) const = 0;

	// Getters
	virtual SourceState getState(void) = 0;

	// Setters
	virtual void setAudio(const ISound* sound) = 0;
	virtual void setAudio(const IMusic* music) = 0;
	virtual void setPosition(const glm::vec3 &position) = 0;
	virtual void setVolume(float volume) = 0;
	virtual void setPitch(float pitch) = 0;
};

}
