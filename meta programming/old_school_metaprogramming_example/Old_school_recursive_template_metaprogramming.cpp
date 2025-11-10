#include <iostream>

// this code demo the old recursive template metaprogramming

// general case
template<int n>
struct factorial
{
    static constexpr int val=n*factorial<n-1>::val;
};

//base case
template<>
struct factorial<0>
{
    static constexpr int val=1;
};

int main()
{
    std::cout << factorial<5>::val << '\n';
    // this line 
    // 1. init template factorial with n=5(give me a struct type that has a static memeber val)   
    // 2. ::val acess the static member in the struct
    // means: Look inside the struct Factorial<5> and read its static member val
}
