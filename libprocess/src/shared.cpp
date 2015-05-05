#include "shared.hpp"

#include <cstdio>
#include <cstring>

#include <atomic>
#include <chrono>

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "geometry.hpp"


namespace lm {


const char* Shared::kSegmentName = "/libprocess-shared-data";


// MagnetoData
MagnetoData::MagnetoData()
{
	for (int i = 0; i < MD_SENSOR_COUNT; ++i)
		sensor[i] = Point();

	poi = Point();

	for (int i = 0; i < MD_SENSOR_COUNT * MD_AXIS_COUNT; ++i)
		input[i] = 0.0;

	for (int i = 0; i < MD_SENSOR_COUNT; ++i)
		magnitude[i] = 0.0;

	source_present = false;

	for (int i = 0; i < MD_CIRCLE_COUNT; ++i)
		circle[i] = Circle();

	for (int i = 0; i < 2 * MD_CIRCLE_COUNT; ++i)
		solution[i] = Point();

	result = Point();

	timestamp = std::chrono::steady_clock::now();
}

MagnetoData::MagnetoData(const MagnetoData &other)
{
	memcpy(this, &other, sizeof(MagnetoData));
}

void MagnetoData::set_sensors(const PointVector &s)
{
	for (int i = 0; i < MD_SENSOR_COUNT; ++i)
		sensor[i] = s[i];
}

void MagnetoData::set_poi(const Point& p)
{
	poi = p;
}

void MagnetoData::set_input(const std::vector<double> &in)
{
	for (int i = 0; i < MD_SENSOR_COUNT * MD_AXIS_COUNT; ++i)
		input[i] = in[i];
}

void MagnetoData::set_magnitudes(const std::vector<double> &m)
{
	for (int i = 0; i < MD_SENSOR_COUNT; ++i)
		magnitude[i] = m[i];
}

void MagnetoData::set_source_present(const bool present)
{
	source_present = present;
}

void MagnetoData::set_circles(const CircleVector &c)
{
	for (int i = 0; i < MD_CIRCLE_COUNT; ++i)
		circle[i] = c[i];
}

void MagnetoData::set_solutions(const PointVector &s)
{
	for (int i = 0; i < 2*MD_CIRCLE_COUNT; ++i)
		solution[i] = s[i];
}

void MagnetoData::set_result(const Point &r)
{
	result = r;
}

void MagnetoData::set_timestamp()
{
	timestamp = std::chrono::steady_clock::now();
}

// ShmData
Shared::ShmData::ShmData() :
	lock(ATOMIC_FLAG_INIT),
	process(0),
	visualize(0),
	data()
{

}

// Shared
Shared::Shared() :
	fd_(-1),
	data_(nullptr)
{

}

Shared::~Shared()
{
	unlink();
}

int Shared::init()
{
	//
	// Create/open shared memory segment.
	//
	int fd_ = shm_open(kSegmentName, O_RDWR|O_CREAT,
	                   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	if (fd_ == -1) {
		fprintf(stderr, "Unable to open %s: %s\n", kSegmentName,
		        strerror(errno));
		return -1;
	}

	//
	// Check if segment is already initialized.
	//
	struct stat mem_stat;
	if (fstat(fd_, &mem_stat) != 0) {
		fprintf(stderr, "fstat: %s\n", strerror(errno));
		return -1;
	}

	bool initialized = true;
	if (mem_stat.st_size != sizeof (ShmData)) {
		//
		// Set segment size.
		//
		if (ftruncate(fd_, sizeof (ShmData)) != 0) {
			fprintf(stderr, "Unable to truncate %s to %lu: %s\n",
			        kSegmentName, sizeof (ShmData),
			        strerror(errno));
			return -1;
		}
		initialized = false;
	}

	//
	// Map segment into adress space
	//
	data_ = (ShmData*) mmap(nullptr, sizeof(ShmData), PROT_READ|PROT_WRITE,
	                        MAP_SHARED, fd_, 0);
	if (data_ == MAP_FAILED) {
		fprintf(stderr, "Unable to map %s: %s\n", kSegmentName,
		        strerror(errno));
		return -1;
	}

	//
	// Initialize if necessary.
	//
	if (not initialized) {
		data_->lock.clear();
		data_->process.store(CONN_NONE);
		data_->visualize.store(CONN_ACTIVE);
		data_->data = MagnetoData();
	}

	return 0;
}

int Shared::unlink()
{
	if (data_ == nullptr && fd_ == -1)
		return 0;

	bool unlink = true;
	if (data_->process.load() == CONN_ACTIVE ||
	    data_->visualize.load() == CONN_ACTIVE) {
		unlink = false;
	}

	//
	// Unmap mapped memory.
	//
	if (munmap(data_, sizeof(ShmData)) != 0) {
		fprintf(stderr, "Unable to unmap: %s\n", strerror(errno));
		return -1;
	}
	data_ = nullptr;

	//
	// Unlink shared segment.
	//
	if (unlink) {
		if (shm_unlink(kSegmentName) != 0) {
			fprintf(stderr, "Unable to unlink %s: %s\n", kSegmentName,
				strerror(errno));
			return -1;
		}
		fd_ = -1;
	}

	return 0;
}

void Shared::set_process_state(const ConnectionState& state)
{
	if (data_ != nullptr)
		data_->process.store(state);
}

void Shared::set_visualize_state(const ConnectionState& state)
{
	if (data_ != nullptr)
		data_->visualize.store(state);
}

bool Shared::is_process_connected()
{
	if (data_ != nullptr)
		return (data_->process.load() == CONN_ACTIVE);
	return false;
}

bool Shared::is_visualize_connected()
{
	if (data_ != nullptr)
		return (data_->visualize.load() == CONN_ACTIVE);
	return false;
}

void Shared::set_data(const MagnetoData &new_data)
{
	if (data_ != nullptr) {
		data_->lock.test_and_set();
		data_->data = new_data;
		data_->lock.clear();
	}
}

MagnetoData Shared::get_data()
{
	MagnetoData tmp;

	if (data_ != nullptr) {
		data_->lock.test_and_set();
		tmp = data_->data;
		data_->lock.clear();
	}

	return tmp;
}


} // namespace lm
