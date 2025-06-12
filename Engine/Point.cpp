#include "Point.hpp"
#include <cmath>

namespace Engine {
    Point::Point() : Point(0, 0) {}
    Point::Point(double x, double y) : x(x), y(y) {}
    bool Point::operator==(const Point &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool Point::operator!=(const Point &rhs) const {
        return !operator==(rhs);
    }
    Point Point::operator+(const Point &rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point Point::operator-(const Point &rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
    Point Point::operator*(const double &rhs) const {
        return Point(x * rhs, y * rhs);
    }
    Point Point::operator/(const double &rhs) const {
        return Point(x / rhs, y / rhs);
    }
    Point Point::Normalize() const {
        if (Magnitude() == 0)
            return Point();
        return Point(x, y) / Magnitude();
    }
    double Point::Dot(const Point &rhs) const {
        return x * rhs.x + y * rhs.y;
    }
    double Point::MagnitudeSquared() const {
        return x * x + y * y;
    }
    double Point::Magnitude() const {
        return sqrt(MagnitudeSquared());
    };
    Point operator*(const double &lhs, const Point &rhs) {
        return rhs * lhs;
    }
}
