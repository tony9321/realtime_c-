#include <iostream>
#include "geometry/Point.hpp"
#include "algebra/Rational.hpp"

int main() {
    math::geometry::Point p(3.0, 4.0);
    math::algebra::Rational r(1, 2);

    std::cout << "Point: (" << p.x() << ", " << p.y() << ")\n";
    std::cout << "Rational: " << r.value() << "\n";
}

