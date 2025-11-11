#include <utility>
#include <iostream>

void process(const std::string& s) {std::cout << "lvalue\n";}
void process(std::string&& s) {std::cout << "rvalue\n";}

template<typename T>
void relay(T&& arg)
{
	process(std::forward<T>(arg));
}

int main()
{
	std::string s="hi";
	relay(s); // lvalue->prints lvalue
	relay("hello"); // rvalue->prints rvalue
}