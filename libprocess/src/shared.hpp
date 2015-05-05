#ifndef _LIBPROCESS_SHARED_H_
#define _LIBPROCESS_SHARED_H_

#include <atomic>
#include <chrono>
#include <vector>

#include "geometry.hpp"

namespace lm {



//
// constants for three sensors
//					//
#define MD_SENSOR_COUNT                 (3)
#define MD_AXIS_COUNT                   (3)
#define MD_CIRCLE_COUNT                 (3)


struct MagnetoData
{
	Point sensor[MD_SENSOR_COUNT];
	Point poi;

	double input[MD_SENSOR_COUNT * MD_AXIS_COUNT];
	double magnitude[MD_SENSOR_COUNT];

	bool source_present;

	Circle circle[MD_CIRCLE_COUNT];
	Point solution[2 * MD_CIRCLE_COUNT];

	Point result;
	std::chrono::steady_clock::time_point timestamp;

public:
	MagnetoData();
	MagnetoData(const MagnetoData &other);

	void set_sensors(const PointVector &s);
	void set_poi(const Point& p);
	void set_input(const std::vector<double> &in);
	void set_magnitudes(const std::vector<double> &m);
	void set_source_present(const bool present = true);
	void set_circles(const CircleVector &c);
	void set_solutions(const PointVector &s);
	void set_result(const Point &r);
	void set_timestamp();
};


enum ConnectionState {
	CONN_NONE = 0,
	CONN_ACTIVE
};


class Shared
{
	struct ShmData {
		std::atomic_flag lock;
		std::atomic<int> process;
		std::atomic<int> visualize;

		MagnetoData data;

		ShmData();
	};

//static
	const static char* kSegmentName;

//
	int fd_;
	ShmData* data_;

public:

	Shared();
	~Shared();

	int init();
	int unlink();

	void set_process_state(const ConnectionState& state);
	void set_visualize_state(const ConnectionState& state);
	bool is_process_connected();
	bool is_visualize_connected();

	void set_data(const MagnetoData &data);
	MagnetoData get_data();
};


} // namespace lm


#endif // _LIBPROCESS_SHARED_H_
