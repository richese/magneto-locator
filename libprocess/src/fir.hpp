#ifndef _LIBPROCESS_FIR_H_
#define _LIBPROCESS_FIR_H_

#include <vector>


namespace lm {


class Fir
{
	std::vector<double> array_;
	const int length_;
	int index_;
	double sum_;
	double val_;

public:
	Fir(const int length, const double val);

	double update(const double val);
	inline double get_val() const { return val_; }
};


} // namespace lm


#endif // _LIBPROCESS_FIR_H_
