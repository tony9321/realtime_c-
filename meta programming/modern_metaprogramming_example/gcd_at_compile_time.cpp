#include <concepts> // std::integral: all the built in int type
#include <utility> // for std::exchange
#include <iostream> 

// this code uses c++20's concept's std::integral and metaprogramming and left fold expression

// compute gcd of two numbers
constexpr auto gcd2 = [] (auto a, auto b)
{
    while(b) a=std::exchange(b, a%b); 
    /*
    std::exchange is the same as:
    1. Stores the old value of x somewhere.
    2. Assigns y into x.
    3. Returns the old value.

    template<class T, class U = T>
    T exchange(T& obj, U&& new_value) {
        T old = std::move(obj);   // save old
        obj = std::forward<U>(new_value); // assign new
        return old;               // give old value back
    }

    ex: int old = std::exchange(a, 10); is int old=a; a=10;
    */
    return a;
};

// compute gcd of any number of int at compile time using fold expression 
template<std::integral T, std::integral... Ts>
constexpr T gcd_all(T first, Ts... rest) {
    return ( ... , gcd2(first, rest) ); // Left fold: gcd2(gcd2(...))
}

int main()
{
    constexpr int g1=gcd_all(48, 18, 30); //6
    /*
    gcd2(gcd2(48, 18), 30)
    (((a op b) op c) op d)
    */
    constexpr int g2 = gcd_all(12, 24, 36, 60); // 12
    std::cout << g1 << ' ' << g2 << '\n';
}
