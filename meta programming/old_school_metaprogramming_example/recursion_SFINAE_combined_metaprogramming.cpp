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