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

#include "Audio/Source.h"

namespace ExoEngine {

	Source::Source()
		: _id(0), _pMusic(nullptr)
	{
		alGenSources(1, &_id);
		alSource3f(_id, AL_POSITION, 0.0f, 0.0f, 0.0f);
	}

	Source::~Source()
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

	void Source::play(void) const
	{
		alSourcePlay(_id);
	}

	void Source::stop(void) const
	{
		alSourceStop(_id);
	}

	void Source::rewind(void) const
	{
		alSourceRewind(_id);
	}

	void Source::streamingUpdate(void) const
	{
		if (_pMusic)
			_pMusic->streamingUpdate(_id);
	}

	// Getters
	Source::SourceState Source::getState(void)
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

	ALuint Source::getSource(void) const
	{
		return _id;
	}

	// Setters
	void Source::setAudio(const Sound* sound)
	{
		alSourcei(_id, AL_BUFFER, ((Sound*)sound)->getBuffer());
	}

	void Source::setAudio(const Music* music)
	{
		if (_pMusic)
		{
			delete _pMusic;
			_pMusic = nullptr;
		}

		_pMusic = (Music*)music;
		alSourceQueueBuffers(_id, 2, _pMusic->getBuffers());
	}

	void Source::setPosition(const glm::vec3& position)
	{
		alSource3f(_id, AL_POSITION, position.x, position.y, position.z);
	}

	void Source::setVolume(float volume)
	{
		alSourcef(_id, AL_GAIN, volume);
	}

	void Source::setPitch(float pitch)
	{
		alSourcef(_id, AL_PITCH, pitch);
	}

}