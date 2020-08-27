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

#include <iostream>
#include <vector>
#include <cstdio>
#include <vorbis/vorbisfile.h>

#ifdef __linux__
# include <AL/al.h>
#else
# include <OpenAL/al.h>
#endif

#define OGG_DEFAULT_BUFFER_SIZE 4096

namespace	ExoAudio
{

class OggLoader
{
public:
	OggLoader(const std::string &filePath);
	~OggLoader(void);

	void readAll(std::vector<char> &buffer, int nbSamples = OGG_DEFAULT_BUFFER_SIZE);
	char* readSample(std::vector<short> &samples, int nbSamples = OGG_DEFAULT_BUFFER_SIZE);

	// Getters
	int getFormat(void) const;
	int getSampleRate(void) const;
	int getTotalRead(void) const;
private:
	OggVorbis_File	_stream;
	FILE*			_file;
	int			 _format;
	int			 _sampleRate;
	int			 _totalRead;
};

}
