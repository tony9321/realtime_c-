## 1. runs at compile time
## 2. Old vs Modern Styles
### Old-school (template-based)
Uses recursion, enable_if, SFINAE(Substitution Failure Is Not An Error)...
#### 1. SFINAE: when compiler has CE to sub. the type args. it just keeps looking instead of throwing CE. So just ditch the bad one [SFINAE.cpp](SFINAE.cpp)
```cpp
#include <iostream>
#include <type_traits>
// SFINAE.cpp
// version for integral types
// std::enable_if_t<condition> only gives a type if condition is true, if it's false, compiler throws it away
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
    printType(5);     // int → prints "Integral type",. the floating point version fails
    printType(3.14);  // double → prints "Floating-point type"
}
```


####  2. Old-school recursive template metaprogramming [Old_school_recursive_template_metaprogramming.cpp](Old_school_recursive_template_metaprogramming.cpp)

```cpp
#include <iostream>

// general case
template<int N>
struct Factorial {
    static constexpr int value = N * Factorial<N - 1>::value;
};

// base case
template<>
struct Factorial<0> {
    static constexpr int value = 1;
};

int main() {
    std::cout << Factorial<5>::value << '\n'; // prints 120
    // !! use ::value here because Factorial<5> is a type so value is it's memeber value
}
```

more particularly it's the same as compiler generate a chain of structs like:
```cpp
struct Factorial<0> {
    static constexpr int value = 1;
};

struct Factorial<1> {
    static constexpr int value = 1 * Factorial<0>::value; // = 1 * 1
};

struct Factorial<2> {
    static constexpr int value = 2 * Factorial<1>::value; // = 2 * 1
};

struct Factorial<3> {
    static constexpr int value = 3 * Factorial<2>::value; // = 3 * 2
};
```
####  3. recursion_SFINAE_combined_metaprogramming [recursion_SFINAE_combined_metaprogramming.cpp](recursion_SFINAE_combined_metaprogramming.cpp)
combining the previous two tricks we have:
```cpp
#include <iostream>
#include <type_traits> // for std::enable_if_t

/* 
	mental model: Printer<Type, GateCondition>
	1st param: templating on T
	2nd param: hidden compile time filster that only activates when condition is true
*/
// primary template: 2 params (T, Enable)
// set Enable defaults to void so we can ignore it normally

template<typename T,typename Enable=void>
struct Printer
{
	static void print(T)
	{
		std::cout << "General type\n";
	}
};

// specialized for integral types
// multiple template params and the second one here is SFINAE filtering — the “is this valid?”
// this one only exist if the enable_if_t is true
// !!! compiler matches specializations by their template param list
template<typename T>
struct Printer<T, std::enable_if_t<std::is_integral_v<T>>>
{
	static void print(T)
	{
		std::cout<< "Integral type\n";
	}
};

int main() {
    Printer<int>::print(42);       // prints "Integral type"
    Printer<double>::print(3.14);  // prints "General type"
}
```
### Modern (constexpr + concepts)
Readable, expressive, uses normal syntax...

We previously saw the old school recursion SFINAE metaprogramming, here is the modern version
#### 1.  [modern_recursion_SFINAE_combined_metaprogramming](modern_recursion_SFINAE_combined_metaprogramming)
```cpp
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
```

Here is another example:
#### 2. gcd: [gcd_at_compile_time.cpp](gcd_at_compile_time.cpp)
```cpp
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
```

## 3. std::exchange
`std::exchange` replaces a value and returns the old one...
  1. Stores the old value of x somewhere. . 
  2. Assigns y into x.
  3. Returns the old value.


It's essentially doing:
```cpp
template<class T, class U = T>
T exchange(T& obj, U&& new_value) {
    T old = std::move(obj);   // save old
    obj = std::forward<U>(new_value); // assign new
    return old;               // give old value back
}
```
note: [move semantics](move%20semantics.md) and [perfect forwarding](perfect%20forward.md)
## 4. Why Old Template Metaprogramming Still Matters
- Type-level computations 
	- constexpr returns value but template can return a type
- Partial specialization
	- compiler can only init the ones that match any of the template type conditions

## 5. Variadic Templates
Templates that can take any number of parameters(compile time for loop)
### Example
```cpp
template<typename... Args>
constexpr auto sum(Args... args) {
    return (args + ...); // folding in cpp17: ((a1 + a2) + a3) + ... loop unrolling in type system
}
```
