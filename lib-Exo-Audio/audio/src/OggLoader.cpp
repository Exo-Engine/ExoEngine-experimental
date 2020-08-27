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

#include "OggLoader.h"

#include <stdexcept>

using namespace	ExoAudio;

OggLoader::OggLoader(const std::string &filePath)
: _file(nullptr), _format(0), _sampleRate(0), _totalRead(0)
{
	// Open for binary reading
	_file = fopen(filePath.c_str(), "rb");
	if (!_file)
		 throw (std::invalid_argument("Error: Ogg file " + filePath + " not found!"));

	// Open stream flux
	if (ov_open(_file, &_stream, nullptr, 0) < 0)
		throw (std::invalid_argument("Error: ogg-vorbis flux corrupted"));

	// Check the number of channels & the frequency of the sampling rate
	vorbis_info *pInfos = ov_info(&_stream, -1);
	_format = pInfos->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	_sampleRate = (int)pInfos->rate;
}

OggLoader::~OggLoader(void)
{
	ov_clear(&_stream);
}

void OggLoader::readAll(std::vector<char> &buffer, int nbSamples)
{
	long bytes = 0;
	std::vector<char> array;

	array.resize(nbSamples);
	do
	{
		bytes = ov_read(&_stream, array.data(), nbSamples, 0, 2, 1, nullptr);
		buffer.insert(buffer.end(), array.begin(), array.begin() + bytes);
	} while (bytes > 0);
}

char* OggLoader::readSample(std::vector<short> &samples, int nbSamples)
{
	int totalSize	= nbSamples * sizeof(short);
	char* samplesPtr = reinterpret_cast<char*>(&samples[0]);
	_totalRead	= 0;

	// Decoding
	while (_totalRead < totalSize)
	{
		int read = (int)ov_read(&_stream, samplesPtr + _totalRead, totalSize - _totalRead, 0, 2, 1, nullptr);

		if (read > 0)
			_totalRead += read;
		else // Error when we reading, stop
			break;
	}

	return samplesPtr;
}

// Getters
int OggLoader::getFormat(void) const
{
	return _format;
}

int OggLoader::getSampleRate(void) const
{
	return _sampleRate;
}

int OggLoader::getTotalRead(void) const
{
	return _totalRead;
}
