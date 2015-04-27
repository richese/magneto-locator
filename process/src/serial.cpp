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
// serial.cpp
//
// Provides simple interface to communicate with devices using serial port.
//
//------------------------------------------------------------------------------
#include "serial.hpp"

#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>


namespace lm {


Serial::Serial() :
	is_open_(false),
	m_fd_(-1)
{

}

Serial::~Serial()
{
	close(m_fd_);
}

int Serial::initialize(const char* portname, const int speed, const int parity,
                       const int blocking)
{
	if (portname == nullptr) {
		fprintf(stderr, "Undefined port name.\n");
		return -1;
	}

	m_fd_ = open(portname, O_RDWR|O_NOCTTY|O_SYNC);
	if (m_fd_ < 0) {
		fprintf(stderr, "Error %d while opening %s: %s\n", errno,
		        portname, strerror(errno));
		return -1;
	}

	if (set_interface_attribs(speed, parity) != 0 ||
	    set_blocking(blocking) != 0)
		return -1;

	is_open_ = true;
	return 0;
}

void Serial::write(const char c)
{
	::write(m_fd_, &c, 1);
}

char Serial::read()
{
	char c;

	::read(m_fd_, &c, 1);

	return c;
}

std::string Serial::readline()
{
	char c;
	std::string line;

	do {
		::read(m_fd_, &c, 1);
		line.push_back(c);
	} while(c != '\n');

	return line;
}

void Serial::flush()
{
	::tcflush(m_fd_, TCIOFLUSH);
}

void Serial::wait_for(const char c)
{
	char ci;

	do {
		::read(m_fd_, &ci, 1);
	} while(ci != c);
}

bool Serial::is_open() const
{
	return is_open_;
}

int Serial::set_interface_attribs(int speed, int parity)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (m_fd_, &tty) != 0)
		goto ssia_error;

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
	// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
	// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (m_fd_, TCSANOW, &tty) != 0)
		goto ssia_error;

	return 0;

ssia_error:
	fprintf(stderr, "Error %d from tcgetattr.\n", errno);
	return -1;
}

int Serial::set_blocking(int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (m_fd_, &tty) != 0)
		goto ssb_error;

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

	if (tcsetattr (m_fd_, TCSANOW, &tty) != 0)
		goto ssb_error;

	return 0;

ssb_error:
	fprintf(stderr, "Error %d while setting term attributes.\n", errno);
	return -1;
}


} // namespace lm
