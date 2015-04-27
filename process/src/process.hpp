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
// process.hpp
//
//
//
//------------------------------------------------------------------------------
#ifndef _PROCESS_PROCESS_H_
#define _PROCESS_PROCESS_H_

#include <string>
#include <vector>

#include "geometry.hpp"


namespace lm {


int parse_raw_data(const std::string &raw_data, const int sensor_cnt,
                   std::vector<double> *out_data);


class Process
{
	bool initialized_;
	PointVector sensors_;
	CircleVector circles_;
	PointVector points_;

protected:
	std::vector<double> input_;

public:
	Process();
	~Process();

	int initialize(const PointVector &sensors);

	int process(const std::string &raw_data);
	int process(const std::vector<double> &data);

	bool is_initialized() const { return initialized_; }
	size_t get_sensor_cnt() const {return sensors_.size(); }
	std::vector<double> get_input() const { return input_; }
	CircleVector get_circles() const { return circles_; }
	PointVector get_points() const { return points_; }

	void clear();

protected:
	int process_common();

private:
	Circle make_circle(const size_t i, const size_t j);
	double get_ratio(const size_t i, const size_t j);
	void make_circles();
	void make_points();
};


class FilteredProcess : public Process
{
	std::vector<double> environment_;

public:
	FilteredProcess();
	~FilteredProcess();

	int initialize(const PointVector &sensors);

	int calibrate(const std::string &raw_data, const double speed);
	int calibrate(const std::vector<double> &data, const double speed);

	int process(const std::string &raw_data);
	int process(const std::vector<double> &data);

	bool is_source_present(const std::vector<double> &input,
	                       const double treshold);

	inline std::vector<double> get_environment() const { return environment_; }

private:
	int calibrate_common(const std::vector<double> &data,
	                     const double speed);
};


} // namespace lm


#endif // _PROCESS_PROCESS_H_
