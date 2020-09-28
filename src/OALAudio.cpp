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

#include "OALAudio.h"

#include <stdexcept>

using namespace	ExoAudioOpenAL;
using namespace	ExoAudio;

static OALAudio*	g_instance = nullptr;

OALAudio&	OALAudio::Get(void)
{
	if (!g_instance)
		g_instance = new OALAudio();
	return (*g_instance);
}

OALAudio::OALAudio(void)
: _pDevice(nullptr), _pContext(nullptr)
{	}

OALAudio::~OALAudio(void)
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(_pContext);
	alcCloseDevice(_pDevice);
}

void OALAudio::initialize(void)
{
	_pDevice = alcOpenDevice(nullptr); // Default device
	if (!_pDevice)
		throw (std::runtime_error("OpenAL error when opening the default device"));

	_pContext = alcCreateContext(_pDevice, nullptr);
	if (!_pContext)
		throw (std::runtime_error("OpenAL error when creating the context"));

	// Activate context
	if (!alcMakeContextCurrent(_pContext))
		throw (std::runtime_error("OpenAL error when activating the context"));

	// Static orientation for 2D, look at Z
	ALfloat Orientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
	alListenerfv(AL_ORIENTATION, Orientation);

	// Default position
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alListenerf(AL_GAIN, 1.0f);
}

void OALAudio::getDevices(std::vector<std::string> &devices)
{
	// Clear list
	devices.clear();

	// Get all devices
	const ALCchar* deviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);

	// Write in vector
	if (deviceList)
	{
		while (strlen(deviceList) > 0)
		{
			devices.push_back(deviceList);
			deviceList += strlen(deviceList) + 1;
		}
	}
}

void OALAudio::updateListener(const glm::vec3 &position, const glm::vec3 &velocity, float volume)
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	alListenerf(AL_GAIN, volume == 0.0f ? 0.000001f : volume);
}

void OALAudio::updateVolume(float volume)
{
	alListenerf(AL_GAIN, volume == 0.0f ? 0.000001f : volume);
}

ISource* OALAudio::createSource(void)
{
	return new OALSource();
}

ISound* OALAudio::createSound(const std::string& filePath)
{
	return new OALSound(filePath);
}

IMusic* OALAudio::createMusic(const std::string& filePath)
{
	return new OALMusic(filePath);
}
