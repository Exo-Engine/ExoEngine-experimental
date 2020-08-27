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

#include "OALMusic.h"

using namespace	ExoAudioOpenAL;
using namespace	ExoAudio;

OALMusic::OALMusic(const std::string &filePath)
: _pOggLoader(nullptr)
{
	// Create OpenAL Buffer
	alGenBuffers(2, _id);

	// Load
	_pOggLoader = new OggLoader(filePath);

	std::vector<short> samples(44100);
	alBufferData(_id[0], _pOggLoader->getFormat(), &_pOggLoader->readSample(samples, 44100)[0], _pOggLoader->getTotalRead(), _pOggLoader->getSampleRate());
	samples.clear();
	alBufferData(_id[1], _pOggLoader->getFormat(), &_pOggLoader->readSample(samples, 44100)[0], _pOggLoader->getTotalRead(), _pOggLoader->getSampleRate());
}

OALMusic::~OALMusic(void)
{
	if (_pOggLoader)
		delete _pOggLoader;

	alDeleteBuffers(2, _id);
}

void OALMusic::streamingUpdate(ALuint source)
{
	ALint status;
	alGetSourcei(source, AL_SOURCE_STATE, &status);

	if (status == AL_PLAYING)
	{
		ALint nbProcessed;
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &nbProcessed);

		for (ALint i = 0; i < nbProcessed; ++i)
		{
			ALuint bufferId;
			alSourceUnqueueBuffers(source, 1, &bufferId);

			std::vector<short> samples(44100);
			alBufferData(bufferId, _pOggLoader->getFormat(), &_pOggLoader->readSample(samples, 44100)[0], _pOggLoader->getTotalRead(), _pOggLoader->getSampleRate());

			alSourceQueueBuffers(source, 1, &bufferId);
		}
	}
}

// Getters
ALuint* OALMusic::getBuffers(void)
{
	return _id;
}
