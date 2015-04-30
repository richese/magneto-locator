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
// geometry.cpp
//
//
//
//------------------------------------------------------------------------------
#include "geometry.hpp"

#include <cmath>

#include <iostream>
#include <vector>


namespace lm {


// Point
// konstruktory
Point::Point() :
	Point(0.0, 0.0)
{

}

Point::Point(const double& x, const double& y) :
	x(x),
	y(y)
{

}

Point::Point(const Point& other)
{
	x = other.x;
	y = other.y;
}

// scitavanie
Point Point::operator + (const Point& other)
{
	return Point(x + other.x, y + other.y);
}

void Point::operator += (const Point& other)
{
	x += other.x;
	y += other.y;
}

Point operator + (const Point& a, const Point& b)
{
	return Point(a.x + b.x, a.y + b.y);
}

// odcitavanie
Point Point::operator - (const Point& other)
{
	return Point(x - other.x, y - other.y);
}

void Point::operator -= (const Point& other)
{
	x -= other.x;
	y -= other.y;
}

Point operator - (const Point& a, const Point& b)
{
	return Point(a.x - b.x, a.y - b.y);
}

// nasobenie konstantou
Point operator * (const double& d, const Point& p)
{
	return Point(d * p.x, d * p.y);
}

// delenie konstantov
Point operator / (const Point& p, const double& d)
{
	return Point(p.x / d, p.y / d);
}

Point operator / (const double& d, const Point& p)
{
	return Point(d / p.x, d / p.y);
}

// vypocet vzdialenosti
double Point::dist(const Point& other) const
{
	Point p = *this - other;
	return std::sqrt( p.x * p.x + p.y * p.y);
}

double Point::dist(const double& x, const double& y) const
{
	return this->dist(Point(x, y));
}

// vypocet stredu medzi dvomi bodmi
Point Point::midpoint(const Point& other) const
{
	Point diff = *this - other;

	return *this + (diff / 2.0);
}

// Circle
// konstruktory
Circle::Circle() :
	Circle(0.0, 0.0, 1.0)
{

}

Circle::Circle(const Point& c, const double& r) :
	c_(c),
	r_(r)
{

}

Circle::Circle(const double& cx, const double& cy, const double& r) :
	Circle(Point(cx,cy), r)
{

}

Circle::Circle(const Circle& other)
{
	c_ = other.c_;
	r_ = other.r_;
}

Point Circle::center() const
{
	return c_;
}

double Circle::radius() const
{
	return r_;
}

// http://paulbourke.net/geometry/circlesphere/
PointVector Circle::intersection(const Circle& other,
                                 IntersectionType *result_type) const
{
	double d = dist(c_, other.c_);

	if (d > (r_ + other.r_)) { // circles are separated
		if (result_type != nullptr)
			*result_type = kSeparate;
		return PointVector();
	}

	if (d < std::abs(r_ - other.r_)) { // contained circle
		if (result_type != nullptr)
			*result_type = kContained;
		return PointVector();
	}

	if ((d == 0.0) && (r_ == other.r_)) { // identical circles
		if (result_type != nullptr)
			*result_type = kIdentical;
		return PointVector();
	}

	double a = (r_ * r_ - (other.r_*other.r_) + d * d) / (2.0 * d);
	double h = std::sqrt(r_ * r_ - a * a);

	Point p2 = c_ + (a / d) * Point(other.c_ - c_);

	if (h == 0) { // circles intersect in one point
		if (result_type != nullptr)
			*result_type = kOnePoint;
		return PointVector() = { p2 };
	}

	if (result_type != nullptr)
			*result_type = kTwoPoints;
	return PointVector() = { // circles intersect in two points
		Point(p2.x + h * (other.c_.y - c_.y) / d,
		       p2.y - h * (other.c_.x - c_.x) / d),
		Point(p2.x - h * (other.c_.y - c_.y) / d,
		       p2.y + h * (other.c_.x - c_.x) / d)
	};
}

Triangle::Triangle(const PointVector &points) :
	points_(),
	area_(0.0)
{
	for (int i=0; i < 3; ++i)
		points_[i] = points[i];

	area_ = 0.5 * (-points_[1].y * points_[2].x + points_[0].y *
	        (-points_[1].x + points_[2].x) + points_[0].x * (points_[1].y -
	        points_[2].y) + points_[1].x * points_[2].y);
}

const Point & Triangle::operator [] (int i) const
{
	return points_[i];
}

double Triangle::area() const
{
	return area_;
}

bool Triangle::is_inside(const Point &p) const
{
	const double s = 1.0 / (2.0 * area_) * (points_[0].y * points_[2].x -
	                 points_[0].x * points_[2].y + (points_[2].y -
	                 points_[0].y) * p.x + (points_[0].x -
	                 points_[2].x)*p.y);

	const double t = 1.0 / (2.0 * area_) * (points_[0].x * points_[1].y -
	                 points_[0].y * points_[1].x + (points_[0].y -
	                 points_[1].y) * p.x + (points_[1].x -
	                 points_[0].x) * p.y);

	return (s > 0.0) and (t > 0.0) and (1.0-s-t > 0.0);
}


double angle(const Point& p1, const Point& p2)
{
	Point p = p2 - p1;
	double angle = std::atan2(p.y, p.x);
	return (angle < 0.0) ? 2 * M_PI+angle : angle;
}

double angle_deg(const Point& p1, const Point& p2)
{
	return 180 * angle(p1, p2) / M_PI;
}

double dist(const Point& p1, const Point& p2)
{
	return p1.dist(p2);
}

Point midpoint(const Point& p1, const Point& p2)
{
	return p1.midpoint(p2);
}

std::ostream& operator << (std::ostream& os, const Point& p)
{
	os << '(' << p.x << ',' << p.y << ')';
	return os;
}

std::ostream& operator << (std::ostream& os, const Circle& c)
{
	os << "C[" << c.center() << ',' << c.radius() << ']';
	return os;
}

void print_circles(const CircleVector &circles)
{
	if (circles.size() == 0)
		return;

	for (auto circle : circles)
		std::cerr << circle << " ";
	std::cerr << std::endl;
}

void print_points(const PointVector &points)
{
	if (points.size() == 0)
		return;

	for (auto point : points)
		std::cerr << point << " ";
	std::cerr << std::endl;
}

void print_doublevector(const std::vector<double> &vector)
{
	if (vector.size() == 0)
		return;

	for(auto d : vector)
		std::cerr << d << " ";
	std::cerr << std::endl;
}


} // namespace lm
