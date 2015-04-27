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
// process.cpp
//
//
//
//------------------------------------------------------------------------------
#include "process.hpp"

#include <cmath>
#include <cstdio>

#include <string>
#include <stdexcept>
#include <vector>

#include "geometry.hpp"


namespace lm {


Process::Process() :
	initialized_(false),
	sensors_(),
	circles_(),
	points_(),
	input_()
{

}

Process::~Process()
{

}

int Process::initialize(const PointVector &sensors)
{
	if (sensors.size() < 3) {
		fprintf(stderr, "Process needs at least 3 sensors.\n");
		return -1;
	}

	sensors_ = PointVector(sensors);
	clear();
	initialized_ = true;
	return 0;
}

int Process::process(const std::string &raw_data)
{
	if (!initialized_)
		return-1;

	if (parse_raw_data(raw_data, sensors_.size(), &input_) != 0)
		return -1;

	return process_common();
}

int Process::process(const std::vector<double> &data)
{
	if (!initialized_)
		return -1;

	if (data.size() != sensors_.size()) {
		fprintf(stderr, "Invalid input data size.\n");
		return -1;
	}
	input_ = data;

	return process_common();
}

int Process::process_common()
{
	circles_.clear();
	points_.clear();

	make_circles();
	make_points();

	return 0;
}

void Process::clear()
{
	input_ = std::vector<double>();
	circles_ = CircleVector();
	points_ = PointVector();
}

Circle Process::make_circle(const size_t i, const size_t j)
{
	using std::pow;
	using std::sqrt;

	const Point &p = sensors_[i];
	const Point &q = sensors_[j];
	const double kk = pow(get_ratio(i, j), 2);
	const double men = kk-1.0;

	const double cx = (kk*q.x - p.x) / men;
	const double cy = (kk*q.y - p.y) / men;
	const double r2 =   pow(cx, 2) + pow(cy, 2)
	                  - ( kk * (pow(q.x, 2) + pow(q.y, 2))
	                      - pow(p.x, 2) - pow(p.y, 2)
	                    ) / men;

	return Circle(cx, cy, sqrt(r2));
}

double Process::get_ratio(const size_t i, const size_t j)
{
	//return std::cbrt(input_[j]) / std::cbrt(input_[i]);
	return std::cbrt(input_[i]) / std::cbrt(input_[j]);
}

void Process::make_circles()
{
	for (size_t i = 0; i < get_sensor_cnt()-1; ++i)
		for (size_t j = i+1; j < get_sensor_cnt(); ++j)
			circles_.push_back(make_circle(i, j));
}

void Process::make_points()
{
	const auto circle_cnt = circles_.size();

	if (circle_cnt < 2)
		return;

	for (size_t i = 0; i < circle_cnt - 1; ++i) {
		for (size_t j = i + 1; j < circle_cnt; ++j) {
			auto intersec = circles_[i].intersection(circles_[j],
			                                         nullptr);
			for (auto p : intersec)
				points_.push_back(p);
		}
	}
}

// FilteredProcess
FilteredProcess::FilteredProcess() :
	Process(),
	environment_()
{

}

FilteredProcess::~FilteredProcess()
{

}

int FilteredProcess::initialize(const PointVector &sensors)
{
	if (Process::initialize(sensors) != 0)
		return -1;

	environment_ = std::vector<double>(get_sensor_cnt(), 0.0);

	return 0;
}

int FilteredProcess::calibrate(const std::string &raw_data, const double speed)
{
	std::vector<double> data;
	if (parse_raw_data(raw_data, get_sensor_cnt(), &data) != 0)
		return -1;

	return calibrate_common(data, speed);
}

int FilteredProcess::calibrate(const std::vector<double> &data,
                               const double speed)
{
	if (data.size() != get_sensor_cnt())
		return -1;

	return calibrate_common(data, speed);
}

int FilteredProcess::calibrate_common(const std::vector<double> &data,
                                      const double speed)
{
	if (!is_initialized())
		return -1;

	if (speed <= 0.0 || speed > 1.0) {
		fprintf(stderr, "Invalid speed range.\n");
		return -1;
	}

	using std::vector;
	for (vector<double>::size_type i = 0; i < get_sensor_cnt(); ++i) {
		environment_[i] = environment_[i] * (1.0 - speed)
		                  + data[i] * speed;
	}

	return 0;
}

int FilteredProcess::process(const std::string &raw_data)
{
	if (!is_initialized())
		return -1;

	std::vector<double> parsed_input;
	if (parse_raw_data(raw_data, get_sensor_cnt(), &parsed_input) != 0)
		return -1;

	for (size_t i = 0; i < get_sensor_cnt(); ++i)
		input_[i] = std::abs(parsed_input[i] -environment_[i]);

	return process_common();
}

int FilteredProcess::process(const std::vector<double> &data)
{
	if (!is_initialized())
		return -1;

	if (data.size() != get_sensor_cnt())
		return -1;

	input_ = std::vector<double>(get_sensor_cnt());
	for (size_t i = 0; i < get_sensor_cnt(); ++i)
		input_[i] = std::abs(data[i] - environment_[i]);

	return process_common();
}

bool FilteredProcess::is_source_present(const std::vector<double> &input,
                                        const double treshold)
{
	if (input.size() != get_sensor_cnt())
		return false;

	for (size_t i=0; i < get_sensor_cnt(); ++i) {
		double diff = input[i] - environment_[i];
		if (std::abs(diff) < treshold)
			return false;
	}
	return true;
}


int parse_raw_data(const std::string &raw_data, const int sensor_cnt,
                   std::vector<double> *out_data)
{
	if (sensor_cnt <= 0)
		return -1;

	if (out_data == nullptr)
		return -1;

	*out_data = std::vector<double>(sensor_cnt);

	std::string data = raw_data;
	for (int i = 0; i < sensor_cnt; ++i) {
		double magnitude = 0.0;
		for (int j = 0; j < 3; ++j) {
			std::string::size_type pos = 0;
			int axis_value = 0.0;

			try {
				axis_value = std::stoi(data, &pos);
			} catch (const std::invalid_argument &ia) {
				fprintf(stderr, "Parsing error: %s\n",
				        ia.what());
				return -1;
			} catch (const std::out_of_range &oor) {
				fprintf(stderr, "Parsing error: %s\n",
				        oor.what());
				return -1;
			}

			// check for sensor saturation
			if(std::abs(axis_value) > 4090) {
				fprintf(stderr,
				        "Sensor %d axis %d is saturated.\n",
				        i, j);
				return 1;
			}

			magnitude += axis_value*axis_value;
			data = data.substr(pos);
		}
		(*out_data)[i] = std::sqrt(magnitude);
	}

	return 0;
}


} // namespace lm
