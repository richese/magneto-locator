#include <iostream>
#include <vector>
#include <valarray>
#include <cmath>

#include "sensor.hpp"

using std::cerr;
using std::endl;


lm::CSensor::CSensor() :
    magnitude(0.0, 3),
    offset(0.0, 3),
    gain(1),
    calibration(true),
    cal_coeff(0.8),
    active(false),
    treshold(1.0),
    position(0.0, 2),
    rot(0.0)
{

}

lm::CSensor::CSensor(const std::valarray<double>& position, int gain,
                 double cal_coeff, double treshold) :
    CSensor()
{

}

lm::CSensor::~CSensor()
{

}

void lm::CSensor::SetPosition(double x, double y)
{
    double init[2] = {x, y};
    position = std::valarray<double>(init, 2);
}

void lm::CSensor::SetRotation(double rot)
{
    this->rot = rot;
}

void lm::CSensor::SetCalibrationCoeff(double coeff)
{
    this->cal_coeff = coeff;
}

std::valarray<double> lm::CSensor::GetPosition() const
{
    return position;
}

double lm::CSensor::GetRotation() const
{
    return rot;
}

void lm::CSensor::Process(const std::valarray<double>& raw_data)
{
    if(raw_data.size() != 3) {
        cerr << "Invalid vector size. Expected 3, got " << raw_data.size() << endl;
        return;
    }

    magnitude = raw_data;
    if(calibration) {
        offset = cal_coeff*offset + (1.0-cal_coeff)*raw_data;
    }
    else {

    }
}

double lm::CSensor::Magnitude()
{
    return sqrt((magnitude*magnitude).sum());
}


lm::CSensorArray::CSensorArray() :
    _s(3),
    _t(0.0, 2),
    _a(0.0),
    _ra(0)
{

}

lm::CSensorArray::~CSensorArray()
{
    _s.clear();
}

void lm::CSensorArray::Add(CSensor& s)
{
    _s.push_back(s);
}

void lm::CSensorArray::SetTranslation(double x, double y)
{
    _t[0] = x;
    _t[1] = y;
}

void lm::CSensorArray::SetTranslation(std::valarray<double>& t)
{
    _t = t;
}

void lm::CSensorArray::SetRotation(size_t around, double a)
{
    _a = a;
    _ra = around;
}

std::valarray<double> lm::CSensorArray::GetPosition(size_t sensor)
{
    if(sensor == _ra) {
        return (_t + _s[_ra].GetPosition());
    }

    std::valarray<double> out = _s[sensor].GetPosition();
    std::valarray<double> delta = out - _s[_ra].GetPosition();

    double angle = _a + atan2(delta[1], delta[0]);

    out[0] *= cos(angle);
    out[1] *= sin(angle);

    return (_t + out + _s[_ra].GetPosition());
}

size_t lm::CSensorArray::Count() const
{
    return _s.size();
}

// TODO: aplikuj posun
lm::CSensor& lm::CSensorArray::operator [] (const size_t i)
{
    return _s[i];
}
