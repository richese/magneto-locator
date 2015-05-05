#include "fir.hpp"

#include <vector>


namespace lm {

Fir::Fir(const int length, const double val) :
	array_(length, val),
	length_(length),
	index_(0),
	sum_(length * val),
	val_(val)
{

}

double Fir::update(const double val)
{
	sum_ -= array_[index_];
	array_[index_] = val;
	sum_ += array_[index_];
	val_ = sum_ / length_;
	index_ = (index_ + 1) % length_;

	return val_;
}


} // namespace lm
