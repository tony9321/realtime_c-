#include <iostream>
#include <type_traits>

// this file demo Substitution Failure Is Not An Error

// version for integral types
template<typename T>
std::enable_if_t<std::is_integral_v<T>>
printType(T) {
    std::cout << "Integral type\n";
}

// version for floating-point types
template<typename T>
std::enable_if_t<std::is_floating_point_v<T>>
printType(T) {
    std::cout << "Floating-point type\n";
}

int main() {
    printType(5);     // int → prints "Integral type"
    printType(3.14);  // double → prints "Floating-point type"
}