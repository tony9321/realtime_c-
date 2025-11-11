## 1. steal resources, leave source in valid but unspecified state. O(1)
Motivations: in cpp11 and prior, this is really resource intensive
```cpp
std::vector<int> makeVec() {
    std::vector<int> v(1'000'000);
    return v; // old days: deep copy of a million ints
}
```

`std::move` transfer ownership when possible:
```cpp
std::string a = "hello";
std::string b = std::move(a); // move, not copy
// b == "hello", a == "" (empty, but still valid)
```
## 2. doesn’t move anything itself, just a cast to an rvalue reference  `T&&`
### ravlue reference 
`T&&` binds to temporary objs(things w/out names and bout to be destroyed)
Also known as temporary
```cpp
int&& x = 5; // OK — 5 is an rvalue (temporary)
int&& y = foo(); // OK if foo() returns a temporary
```
however: can't bind rvalue reference to a named var:
```cpp
int a = 10;
int&& b = a; // error, a is an lvalue
```
In other words: `T&&` means it can take ownership of something bc/ it's going to be destroyed soon
#### Lifetime of temporary: 
Normally: ends at the end of the full expression that created it
```cpp
void f(const std::string& s) { }
f(std::string("hello")); // temporary lives until f() finishes
```
#### `&&` in template: forwarding references(universal reference, diff from normal `T&&` reference collapsing)
template deduce what each of the passed in param should be 
```cpp
template<class T>
void foo(T&& x) { /* ... */ }
```
`x`is a forwarding ref:
- If pass lvalue: `T` becomes `int&`, so `T&&` collapses to `int&`
- If you pass an rvalue : `T` becomes `int`, so `T&&` stays `int&&`
so we can do:
```cpp
int n = 10;
foo(n);    // T = int&, param type = int&
foo(5);    // T = int,   param type = int&&
```
### lvalue reference `T&` 
binds to a named var(that is in mem.)
```cpp
int a = 10;
int& ref = a; // OK, a is an lvalue
```
however: can't bind a non-const lvalue reference to a temporary:
```cpp
int& r = 5; // Error, 5 has no addressable storage
```
### TLDR: lvalue`T&`: refers to existing named var, rvalue `T&&`: refers to a temporary
| Symbol                | Name                 | Binds to                            | Used for                        |
| --------------------- | -------------------- | ----------------------------------- | ------------------------------- |
| `T&`                  | lvalue reference     | Named objects (have an address)     | Normal references, modification |
| `T&&`                 | rvalue reference     | Temporaries (no name, about to die) | Move semantics                  |
| `T&&` *(in template)* | forwarding reference | Both lvalues and rvalues            | Perfect forwarding              |

## 3. move ctor
A special ctor that transfer resources from an obj to another
```cpp
Buffer(Buffer&& other) noexcept:data(other.data), size(other.size)
{
	// leaves `other` in a _valid but empty_ state so its destructor won’t free the memory twice
    other.data = nullptr; 
    other.size = 0;   
}
```
`Buffer&& other`: rvalue ref, binds to tmp or obj we cast w/ move(we can steal its stuff bc/ it will be destroyed soon)
`:data(other.data), size(other.size)`: steals `other`'s ptr and size

move ctor is used automatically in:
###### 1. ret a local obj from func:
```cpp
Buffer makeBuffer() {
    Buffer b(10);
    return b;  // compiler uses move ctor
}
```
###### 2. Passing a temporary:
```cpp
Buffer b2 = Buffer(20);  // move from temporary
```
### `std::move and move ctor example` [move_and_move_ctor.cpp](move_and_move_ctor.cpp)
```cpp
#include <iostream>
#include <utility>
#include <string>

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
	A a1(s); // copy, constructed
	//!! type of std::move(s)=std::string&&, compiler still takes it as std::string
	// but this time two moves no copy
	A a2(std::move(s)); //move but will still say constructed
	A a3(std::move(a1)); // now 'a1' is an A, so A(A&&) matches, moves
}
```
###### side note: `noexcept` : guarantee not to throw bc/ std lib use it to decides if move is safe
```cpp
std::vector<T> v;
v.emplace_back(...);
```
when needs to reallocate, it'll move instead of copy elements if move ctor is marked `noexcept`
