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
// main.cpp
//
//
//
//------------------------------------------------------------------------------
#include <cmath>
#include <cstdio>

#include <iostream>
#include <string>
#include <vector>

#include <signal.h>

#include "geometry.hpp"
#include "process.hpp"
#include "serial.hpp"


namespace {


void print_circles(const lm::CircleVector &circles);

void print_points(const lm::PointVector &points);

void print_doublevector(const std::vector<double> &vector);

void int_handler(int signum);


} // namespace


int main()
{
	//
	// Set custom action for ^C.
	//
	struct sigaction int_action;
	int_action.sa_handler = int_handler;
	int_action.sa_flags = 0;

	sigaction(SIGINT, &int_action, nullptr);

	//
	// Open connection to embbeded data collector.
	//
	const char* port = "/dev/ttyACM0";
	lm::Serial tiva;
	if (tiva.initialize(port) != 0) {
		fprintf(stderr, "Error while opnening serial port at %s.\n",
		        port);
		return -1;
	}
	fprintf(stderr, "Opened connection to %s\n", port);

	//
	// Define sensor positions in plane.
	//
	const lm::PointVector sensors = {
		lm::Point(-3, 0),
		lm::Point(3, 0),
		lm::Point(0, std::sqrt(6 * 6 - 3 * 3))
	};
	fprintf(stderr, "Sensor layout: ");
	print_points(sensors);

	//
	// Initialize mathematical model for given sensor layout.
	//
	lm::FilteredProcess proc;
	if (proc.initialize(sensors) != 0) {
		fprintf(stderr, "Error while initializing Process instance.\n");
		return -1;
	}

	lm::Triangle triangle(sensors);

	//
	// Flush any accumulated input data and sync with data collector.
	//
	fprintf(stderr, "Syncing... Flushing.\n");
	tiva.flush();
	fprintf(stderr, "Syncing... Waiting. \n");
	tiva.wait_for('\n');
	fprintf(stderr, "Syncing... Done.    \n");

	//
	// Calibrate.
	//
	const int kCalibrationLoops = 25;
	const double kSpeedFactor = 0.01;
	double speed = 0.95;

	fprintf(stderr, "Calibrating... ");
	for (int i = 0; i < kCalibrationLoops; ++i) {
		if (proc.calibrate(tiva.readline(), speed) != 0)
			return -1;
		speed -= kSpeedFactor;

	}
	fprintf(stderr, "Done.\nEnviroment values: ");
	print_doublevector(proc.get_environment());

	//
	// Enter main loop.
	//
	fprintf(stderr, "Entering main loop.\n");
	//for (int loop = 0; loop < 500; ++loop) {
	int loop = 0;
	while (1) {
		++loop;
		proc.clear();
		//
		// Load and parse sensor data.
		// Detect if any sensor is in saturation.
		//
		std::string raw_data = tiva.readline();
		std::vector<double> proc_input;
		int parse_status = lm::parse_raw_data(raw_data,
		                                      proc.get_sensor_cnt(),
		                                      &proc_input);
		switch(parse_status) {
		default:
			break;
		case 1:
			fprintf(stderr, "Skipping loop: sensor saturated.\n");
			continue;
		case -1:
			return -1;
		}

		//
		// Decide whether a magnet is present or not.
		//
		const bool present = proc.is_source_present(proc_input, 30.0);

		if (present) {
			//
			// Create possible solutions for current input data.
			//
			if (proc.process(proc_input) != 0) {
				fprintf(stderr, "Error while processing data.\n");
				continue;
			}

			//
			// Skip loop if some solutions are missing.
			//
			if (proc.get_points().size() != 6) {
				fprintf(stderr, "Skipping loop: missing solutions.\n");
				continue;
			}

			//
			// Eliminate solutions inside of sensor triangle.
			//
			lm::PointVector valid_points;
			for (auto p : proc.get_points()) {
				if (!triangle.is_inside(p))
					valid_points.push_back(p);
			}

			if (valid_points.size() < 3) {
				fprintf(stderr, "This should not happen: missing valid points.\n");
				if (valid_points.size() < 2)
					continue;
			}

			//
			// Compute average from remaining solutions.
			//
			lm::Point result = valid_points.back();
			valid_points.pop_back();
			while (valid_points.size() != 0) {
				result = lm::midpoint(result,
				                      valid_points.back());
				valid_points.pop_back();
			}

			//
			// Print output.
			//
			printf("%i\t\t%lf\t%lf\t%lf\t%lf\n",
			       loop,
			       result.x,
			       result.y,
			       lm::dist(result, lm::Point(0.0, 0.0)),
			       lm::angle_deg(result, lm::Point(0.0, 0.0))
			       );

		} else {
			//
			// Continue calibration at slow speed
			// if no source is present.
			//
			//if (proc.calibrate(proc_input, 0.025))
			//	return -1;
			//proc.clear();
		}
	}

	return 0;
}


namespace {


void int_handler(int signum)
{
	exit(0);
}

void print_circles(const lm::CircleVector &circles)
{
	if (circles.size() == 0)
		return;

	for (auto circle : circles)
		std::cerr << circle << " ";
	std::cerr << std::endl;
}

void print_points(const lm::PointVector &points)
{
	if (points.size() == 0)
		return;

	for (auto point : points)
		std::cerr << point << " ";
	std::cerr << std::endl;
}

void print_doublevector(const std::vector<double> &vector)
{
	if (vector.size() == 0)
		return;

	for(auto d : vector)
		std::cerr << d << " ";
	std::cerr << std::endl;
}


} // namespace
