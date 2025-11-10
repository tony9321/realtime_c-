#include <iostream>
#include <concepts>

template<std::integral T>
void printType(T) {std::cout << "Integral Type\n";}

template<std::floating_point T>
void printType(T) {std::cout << "Floating point type\n";}

int main() {
    printType(42);      // int → integral
    printType(3.14);    // double → floating-point
    printType(2.0f);    // float → floating-point
}
