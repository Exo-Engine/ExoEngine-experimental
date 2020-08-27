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

#ifdef __linux__
# include <AL/al.h>
# include <AL/alc.h>
#else
# include <OpenAL/al.h>
# include <OpenAL/alc.h>
#endif

#include "IAudio.h"
#include "ISound.h"
#include "OALSource.h"
#include "OALSound.h"
#include "OALMusic.h"

namespace	ExoAudioOpenAL
{

class OALAudio : public ExoAudio::IAudio
{
public:
	static OALAudio&	Get(void);

	virtual void initialize(void);

	virtual void getDevices(std::vector<std::string> &devices);
	virtual void updateListener(const glm::vec3 &position, const glm::vec3 &velocity, float volume);
	virtual void updateVolume(float volume);

	virtual ExoAudio::ISource* createSource(void);
	virtual ExoAudio::ISound* createSound(const std::string& filePath);
	virtual ExoAudio::IMusic* createMusic(const std::string& filePath);
private:
	OALAudio(void);
	virtual ~OALAudio(void);
private:
	ALCdevice* _pDevice;
	ALCcontext* _pContext;
};

}
