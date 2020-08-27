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

#include "OALSource.h"

using namespace	ExoAudioOpenAL;
using namespace	ExoAudio;

OALSource::OALSource()
: _id(0), _pMusic(nullptr)
{
	alGenSources(1, &_id);
	alSource3f(_id, AL_POSITION, 0.0f, 0.0f, 0.0f);
}

OALSource::~OALSource()
{
	alSourceStop(_id);

	// Clean source buffers
	if (_pMusic)
	{
		ALint	nbQueued;
		ALuint buffer;
		alGetSourcei(_id, AL_BUFFERS_QUEUED, &nbQueued);

		for (ALint i = 0; i < nbQueued; ++i)
			alSourceUnqueueBuffers(_id, 1, &buffer);

		alSourcei(_id, AL_BUFFER, 0);
	}

	alSourcei(_id, AL_BUFFER, 0);
	alDeleteSources(1, &_id);
}

void OALSource::play(void) const
{
	alSourcePlay(_id);
}

void OALSource::stop(void) const
{
	alSourceStop(_id);
}

void OALSource::rewind(void) const
{
	alSourceRewind(_id);
}

void OALSource::streamingUpdate(void) const
{
	if (_pMusic)
		_pMusic->streamingUpdate(_id);
}

// Getters
ISource::SourceState OALSource::getState(void)
{
	ALint status;
	alGetSourcei(_id, AL_SOURCE_STATE, &status);

	switch (status)
	{
		case AL_STOPPED:
			return SourceState::STOPPED;
		case AL_PLAYING:
			return SourceState::PLAYING;
		case AL_PAUSED:
			return SourceState::PAUSED;
		default:
			return SourceState::INITIAL;
	};
}

ALuint OALSource::getSource(void) const
{
	return _id;
}

// Setters
void OALSource::setAudio(const ISound* sound)
{
	alSourcei(_id, AL_BUFFER, ((OALSound*)sound)->getBuffer());
}

void OALSource::setAudio(const IMusic* music)
{
	if (_pMusic)
	{
		delete _pMusic;
		_pMusic = nullptr;
	}

	_pMusic = (OALMusic*)music;
	alSourceQueueBuffers(_id, 2, _pMusic->getBuffers());
}

void OALSource::setPosition(const glm::vec3 &position)
{
	alSource3f(_id, AL_POSITION, position.x, position.y, position.z);
}

void OALSource::setVolume(float volume)
{
	alSourcef(_id, AL_GAIN, volume);
}

void OALSource::setPitch(float pitch)
{
	alSourcef(_id, AL_PITCH, pitch);
}
