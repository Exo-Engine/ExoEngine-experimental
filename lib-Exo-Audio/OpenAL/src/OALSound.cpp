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

#include "OALSound.h"

#include <stdexcept>

using namespace	ExoAudioOpenAL;
using namespace	ExoAudio;

OALSound::OALSound(const std::string &filePath)
: _id(0)
{
	// Create OpenAL Buffer
	alGenBuffers(1, &_id);

	// Load
	std::vector<char> buffer;
	OggLoader* pOggLoader = new OggLoader(filePath);
	pOggLoader->readAll(buffer);

	// Filling the OpenAL buffer with the read data
	alBufferData(_id, pOggLoader->getFormat(), &buffer[0], (ALsizei)buffer.size(), pOggLoader->getSampleRate());

	delete pOggLoader;
	pOggLoader = nullptr;

	if (alGetError() != AL_NO_ERROR)
		throw (std::invalid_argument("OpenAL error when loading " + filePath));
}

OALSound::~OALSound(void)
{
	alDeleteBuffers(1, &_id);
}

// Getters
ALuint OALSound::getBuffer(void) const
{
	return _id;
}
