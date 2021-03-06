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

namespace ExoEngine
{

	class Message
	{
		public:
			Message(void);
			Message(const Message &src);
			Message(const std::string &src);
			Message(const void *ptr, size_t size);
			Message(size_t size);
			template	<typename T>
			Message(const T &src) : Message((const void *)&src, sizeof(T))
			{
			}
			~Message(void);

			Message	&append(const Message &src);
			Message	&append(const std::string &src);
			Message	&append(const void *ptr, size_t size);
			template	<typename T>
			Message	&append(const T &src)
			{
				return (append((void *)&src, sizeof(T)));
			}

			Message	&operator=(const Message &src);
			Message	&operator=(const std::string &src);
			template	<typename T>
			Message	&operator=(const T &src)
			{
				return ((*this = Message((void *)&src, sizeof(T))));
			}

			Message	&operator+=(const Message &src);
			Message	&operator+=(const std::string &src);
			template	<typename T>
			Message	&operator+=(const T &src)
			{
				return ((*this += Message((void *)&src, sizeof(T))));
			}

			Message	operator+(const Message &src) const;
			Message	operator+(const std::string &src) const;
			template	<typename T>
			Message	operator+(const T &src)
			{
				return ((*this + Message((void *)&src, sizeof(T))));
			}

			uint8_t	&operator[](size_t idx);

			void	resize(size_t size);
			void	clear(void);

			const void	*getPtr(void) const;
			size_t		getSize(void) const;

			std::string	to_string(void) const;
		private:
			std::vector<uint8_t>	_message;
	};

}

std::ostream	&operator<<(std::ostream &out, const ExoEngine::Message &msg);
