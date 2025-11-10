#pragma once
#include <iostream>

namespace math::algebra
{
    class Rational{
        int num_, den_; // private vars
        public:
            Rational(int n, int d); // constructor
            double value() const; // returns n / d
            void print() const; // prints "n/d"
    };
} // namespace algebra
