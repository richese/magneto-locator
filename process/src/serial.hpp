//------------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2015 Lukáš Mandák <lukas.mandak@yandex.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//
// serial.hpp
//
// Provides simple interface to communicate with devices using serial port.
//
//------------------------------------------------------------------------------
#ifndef _PROCESS_SERIAL_H_
#define _PROCESS_SERIAL_H_

#include <string>

#include <termios.h>


namespace lm {


class Serial
{
	bool is_open_;
	int m_fd_;

public:

	Serial();
	~Serial();

	int initialize(const char* portname,
	               const int speed = B115200,
	               const int parity = 0,
	               const int blocking = 0);

	bool is_open() const;
	void write(const char c);
	char read();
	std::string readline();
	void flush();
	void wait_for(const char c);

private:

	int set_blocking(int should_block);
	int set_interface_attribs(int speed, int parity);
};


} // namespace lm


#endif // _PROCESS_SERIAL_H_
