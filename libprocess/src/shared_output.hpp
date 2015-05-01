#ifndef _PROCESS_SHARED_OUTPUT_H_
#define _PROCESS_SHARED_OUTPUT_H_

#include <vector>

#include "geometry.hpp"

namespace lm {


struct SharedData
{
	unsigned int status;
	Point position;

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
