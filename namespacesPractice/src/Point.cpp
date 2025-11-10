 #include "geometry/Point.hpp"
 namespace math::geometry
 {
     Point::Point(double x, double y):x_(x), y_(y) {} // memeber initializer list
     double Point::x() const {return x_;}
     double Point::y() const {return y_;}
     void Point::print() const
     {
         std::cout << "(" << x_ << ", " << y_ << ")\n";
     }
 }
