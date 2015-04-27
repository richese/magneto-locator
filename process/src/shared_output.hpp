#ifndef _PROCESS_SHARED_OUTPUT_H_
#define _PROCESS_SHARED_OUTPUT_H_

#include <vector>

#include "geometry.hpp"

namespace lm {


struct SharedData
{
	// predefined data
	static const size_t kSensorCount = 3;
	static const size_t kSensorAxisCount = 3;
	Point sensors[kSensorCount];
	Point observer;
	// sensor data
	double raw_input[kSensorCount][kSensorAxisCount];
	double magnitudes[kSensorCount];
	// process data
	Circle circles[kSensorCount];
	Point points[2 * kSensorCount];
	//output data
	int state;
	Point position;
	double distance;
	double angle;
	double min_angle;
	double max_angle;

public:
	SharedData();

	int initialize(const PointVector &sensors, const Point &observer);
	int set_sensor_data(const std::vector<double> &raw_input,
	                    const std::vector<double> &magnitudes);
	int set_process_data(const CircleVector &circles,
	                     const PointVector &points);
	int compute_output();

private:
	void lock();
	void unlock();
};


} // namespace lm


#endif // _PROCESS_SHARED_OUTPUT_H_
