#pragma once 
#include<iostream>

namespace math::geometry
{
    class Point
    {
        private: double x_, y_;// private var 
        public:
            Point(double x, double y);
            double x() const; // getter function and const means it won't change the val
            double y() const;
            void print() const; // helper method to print
    };
}// namespace math::geometry
