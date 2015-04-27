#ifndef _SENSOR_H_
#define _SENSOR_H_

#include <array>
#include <vector>
#include <valarray>

#include "geometry.hpp"

namespace lm
{
class CSensor
{
    std::valarray<double> magnitude;
    std::valarray<double> offset;

    int gain;

    bool calibration;
    double cal_coeff;

    bool active;
    double treshold;

    std::valarray<double> position;
    double rot;

public:

    CSensor();
    CSensor(const std::valarray<double>& position, int gain, double cal_coeff, double treshold);
    ~CSensor();

    void SetPosition(double x, double y);
    void SetRotation(double rot);
    void SetCalibrationCoeff(double coeff);
    std::valarray<double> GetPosition() const;
    double GetRotation() const;
    void Process(const std::valarray<double>& raw_data);
    double Magnitude();
};

} // namespace


namespace lm
{
class CSensorArray
{
    std::vector<CSensor> _s;    // sensors
    std::valarray<double> _t;   // translation size
    double _a;                  // rotation size in radians
    size_t _ra;                 // index of sensor to rotate around

public:

    CSensorArray();
    ~CSensorArray();

    void Add(CSensor& s);
    void SetTranslation(double x, double y);
    void SetTranslation(std::valarray<double>& t);
    void SetRotation(size_t around, double a);

    std::valarray<double> GetPosition(size_t sensor);

    size_t Count() const;
    CSensor& operator [] (const size_t i);
};

} //namespace


class CM
{
	std::array<CPoint, 3>  position;
	std::array<double, 3>  magnitude;
	std::array<CCircle, 3> circles;


public:


};


#endif /* _SENSOR_H_ */
