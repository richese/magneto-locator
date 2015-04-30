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

#include <string>

#include <signal.h>

#include "geometry.hpp"
#include "process.hpp"
#include "serial.hpp"


namespace {


void int_handler(int signum);


//
// Define some usefull constants.
//

//!
//! Mountpoint used by LaunchPad tty.
//!
const char* kSerialPort = "/dev/ttyACM0";

//!
//! Position of sensors in plane in the real world.
//! [cm]
//!
const lm::PointVector kSensorPositions = {
	lm::Point(-3, 0),
	lm::Point(3, 0),
	lm::Point(0, std::sqrt(6 * 6 - 3 * 3))
};

//!
//! Used to eliminate points inside of sensor triangle.
//!
const lm::Triangle kSensorTriangle(kSensorPositions);

//!
//! Calibration constants.
//!
const int kCalibrationLoops = 25;
const double kCalibrationSpeedInitial = 0.95;
const double kCalibrationSpeedFactor = 0.01;


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
	// Initialize mathematical model for given sensor layout.
	//
	lm::FilteredProcess proc;
	if (proc.initialize(kSensorPositions) != 0) {
		fprintf(stderr, "Error while initializing Process instance.\n");
		return -1;
	}

	//
	// Open connection to embbeded data collector.
	// Flush any accumulated input data and sync with data collector.
	//
	lm::Serial tiva;
	if (tiva.initialize(kSerialPort) != 0) {
		fprintf(stderr, "Error while opnening serial port at %s.\n",
		        kSerialPort);
		return -1;
	}
	fprintf(stderr, "Opened connection to %s\n", kSerialPort);
	tiva.flush();
	fprintf(stderr, "Syncing... Waiting. \n");
	tiva.wait_for('\n');
	fprintf(stderr, "Syncing... Done.    \n");

	//
	// Calibrate.
	//
	double speed = kCalibrationSpeedInitial;

	fprintf(stderr, "Calibrating... ");
	for (int i = 0; i < kCalibrationLoops; ++i) {
		std::string line;
		if(tiva.readline(&line) != 0)
			return -1;
		if (proc.calibrate(line, speed) != 0)
			return -1;
		speed -= kCalibrationSpeedFactor;

	}
	fprintf(stderr, "Done.\nEnviroment values: ");
	lm::print_doublevector(proc.get_environment());

	//
	// Enter main loop.
	//
	lm::Point fpoint = lm::Point();
	fprintf(stderr, "Entering main loop.\n");
	int loop = 0;
	while (1) {
		++loop;
		proc.clear();
		//
		// Load and parse sensor data.
		// Detect if any sensor is in saturation.
		//
		std::string raw_data;
		if (tiva.readline(&raw_data) != 0)
			return -1;

		std::vector<double> proc_input;
		int parse_status = lm::parse_raw_data(raw_data,
		                                      proc.get_sensor_cnt(),
		                                      &proc_input);
		switch(parse_status) {
		default:
			break;
		case 1:
			//fprintf(stderr, "Skipping loop: sensor saturated.\n");
			continue;
		case -1:
			return -1;
		}

		//
		// Decide whether a magnet is present or not.
		//
		if (!proc.is_source_present(proc_input, 30.0))
			continue;

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
		if (proc.eliminate_triangle(kSensorTriangle) != 0) {
			fprintf(stderr, "Problem while eliminating triangle points.\n");
			continue;
		}

		//
		// Compute average from remaining solutions.
		//
		lm::Point result = proc.average_points();

		//if (loop != 1) {
			//lm::Point tmp = lm::midpoint(result, fpoint);
			//fpoint = result;
			//result = tmp;
		//} else {
			//fpoint = result;
		//}
		//
		// Print output.
		//
		printf("%i\t", loop);
		printf("%.2lf\t%.2lf\t", result.x, result.y);
		printf("%.2lf\t", lm::dist(result, lm::Point()));
		printf("%.2lf°\t", lm::angle_deg(result, lm::Point()));
		putchar('\n');

		//
		// End of main loop.
		//
	}

	return 0;
}


namespace {


void int_handler(int signum)
{
	exit(0);
}


} // namespace
