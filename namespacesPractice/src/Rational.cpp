#include "algebra/Rational.hpp"
namespace math::algebra
{
    Rational::Rational(int n, int d):num_(n), den_(d) {} // memeber initializer list
    double Rational::value() const {return static_cast<double>(num_)/den_;} // static_cast is the modern cpp way to cast it's safer and clearer
    void Rational::print() const
    {
        std::cout << num_  << "/" << den_ << "\n";
    }
} // namespace math::algebra
