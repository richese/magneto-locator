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
// geometry.hpp
//
//
//
//------------------------------------------------------------------------------
#ifndef _PROCESS_GEOMETRY_H_
#define _PROCESS_GEOMETRY_H_

#include <array>
#include <iostream>
#include <vector>


namespace lm {


class Point
{
public:
	double x, y;


	Point();
	Point(const double& x, const double& y);
	Point(const Point& other);


	Point operator + (const Point& other);
	void operator += (const Point& other);
	friend Point operator + (const Point& a, const Point& b);

	Point operator - (const Point& other);
	void operator -= (const Point& other);
	friend Point operator - (const Point& a, const Point& b);

	friend Point operator * (const double& d, const Point& p);

	friend Point operator / (const Point& p, const double& d);
	friend Point operator / (const double& d, const Point& p);

	double dist(const Point& other) const;
	double dist(const double& x, const double& y) const;

	Point midpoint(const Point& other) const;
};

typedef std::vector<lm::Point> PointVector;


class Circle
{
	/** \brief Stred kružnice. */
	Point c_;

	/** \brief Polomer kružnice. */
	double r_;

public:

	Circle();
	Circle(const Point& c, const double& r);
	Circle(const double& cx, const double& cy, const double& r);
	Circle(const Circle& other);

	Point center() const;
	double radius() const;

	enum IntersectionType {
		kTwoPoints = 0,
		kOnePoint,
		kSeparate,
		kContained,
		kIdentical
	};

	PointVector intersection(const Circle& other,
	                         IntersectionType *result_type) const;
};

typedef std::vector<lm::Circle> CircleVector;

//!
//! Body musia byť usporiadane v proticmere hodinovych ruciciek
//!
class Triangle
{
	std::array<Point, 3> points_;
	double area_;

public:
	Triangle(const PointVector &points);

	const Point & operator [] (int i) const;

	double area() const;

	bool is_inside(const Point &p) const;
};


// other functions

/** \brief Vypočíta uhol od bodu p1 ku bodu p2.
 *  \return Hodnota v intervale <0, 2*PI) radianov.
 */
double angle(const Point& p1, const Point& p2);

/** \brief Vypočíta uhol od bodu p1 ku bodu p2.
 *  \return Hodnota v intervale <0, 360) stupňov.
 */
double angle_deg(const Point& p1, const Point& p2);

double dist(const Point& p1, const Point& p2);

Point midpoint(const Point& p1, const Point& p2);

std::ostream& operator << (std::ostream& os, const Point& p);

std::ostream& operator << (std::ostream& os, const Circle& c);

void print_circles(const lm::CircleVector &circles);

void print_points(const lm::PointVector &points);

void print_doublevector(const std::vector<double> &vector);


} // namespace lm

#endif // _PROCESS_GEOMETRY_H_
