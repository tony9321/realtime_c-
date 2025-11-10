#include <iostream>

constexpr unsigned long long factorial(unsigned int n)
{
    return (n<=1)?1:(n*factorial(n-1));
}

int main()
{
    constexpr auto fact5=factorial(5); //computed at compile time
    std::cout<< "Factorial(t)=" << fact5 << "\n";
    unsigned int n;
    std::cout << "Enter a number: ";
    std::cin >> n;
    std::cout << "Factorial(" << n << "): " << factorial(n) << "\n";

}
