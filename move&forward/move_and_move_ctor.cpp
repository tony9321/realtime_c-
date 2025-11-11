#include <iostream>
#include <utility>
#include <string>

// this code demo move ctor and std::move
struct A
{
	std::string data;
	// the move in this one just move the local copied string d to memebr var data, but before that since d is passed in by val it's copied 
	A(std::string d):data(std::move(d)) {std::cout << "Constructed\n";}
	// pure move only 
	A(A&& other) noexcept:data(std::move(other.data)) {std::cout << "moved\n";}
};
int main()
{
	std::string s="world";
	A a1(s); // copy
	//!! type of std::move(s)=std::string&&, compiler still takes it as std::string
	// but this time two moves no copy
	A a2(std::move(s)); //move but will still say constructed
	A a3(std::move(a1)); // now 'a1' is an A, so A(A&&) matches
}