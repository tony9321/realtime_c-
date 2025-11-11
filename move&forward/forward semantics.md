## Conditional cast for perfect forwarding
Want to preserve whether args are lvalue or ravlue since `std::move` always turns things in rvalue. 
### Tiny Example:
```cpp
// func ret a unique_ptr<T>
template<typename T, typename... Args> // variadic templates param pack
std::unique_ptr<T> make_unique_like(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```
- if passed in rvalues, moves them
- if passed in lvalues, keeps them as lvalues(no steal)
`T`is the type we are constructing
`Args...`is param pack (zero or more types)
Each `Args&&... args`is a forward reference(universal reference) bc/ `Args`is a template param:
- If the caller passes an lvalue of type `U`, the corresponding `Args` deduces to `U&`, and the parameter type becomes `U&` (not `U&&`)
- If the caller passes an rvalue of type `U`, the corresponding `Args` deduces to `U`, and the parameter type stays `U&&`
- The trailing `...` after both `Args&&` and `args` says “there’s one of these per argument in the pack
`std::forward<Args>(args)...)`is conditional cast:
	ex:`std::forward<X>(y)`
- If `X` is an lvalue-reference type (e.g., `U&`), `forward` returns `y` as an lvalue
- If `X` is not an lvalue-reference (e.g., `U`), `forward` returns `y` as an rvalue.
- The trailing `...` here **expands** the pack, so it becomes:
```cpp
  new T(std::forward<Arg1>(arg1), std::forward<Arg2>(arg2), /* ... */)
  ```
`std::unique_ptr<T>( /* pointer */ )` is for RAII
#### concrete expansion
- Passing an **lvalue**
```cpp
std::string name = "alice";
auto p = make_unique_like<std::pair<int, std::string>>(42, name);
```

[[move semantics#ravlue reference|notes on rvalue]] [[move semantics#lvalue reference T&|notes on lvalue ]] [[meta programming#5. Variadic Templates|notes on Variadic Templates]] [[timer(RAII_Volatile_InlineASM).cpp|notes on RAII]]  
### Example: [forward_ex.cpp](forward_ex.cpp)
wrapper preserving value category
```cpp
void process(const std::string& s) { std::cout << "lvalue\n"; }
void process(std::string&& s) { std::cout << "rvalue\n"; }

template<typename T>
void relay(T&& arg) {
    process(std::forward<T>(arg));
}

int main() {
    std::string s = "hi";
    relay(s);        // lvalue → prints lvalue
    relay("hello");  // rvalue → prints rvalue
}
```
- If you replace `std::forward<T>(arg)` with `arg` → always lvalue, both print "lvalue"  
- If you replace it with `std::move(arg)` → always rvalue, both print "rvalue"
### Mechanics
conditional `static_cast`
```cpp
template<typename T>
T&& forward(std::remove_reference_t<T>& arg) {
    return static_cast<T&&>(arg);
}
```
- If `T` was deduced as `int&` → becomes `int& &&` → collapses to `int&` (lvalue)
- if `T` was deduced as `int` → becomes `int&&` (rvalue)
##### `std::remove_reference_t`:
return a `T`, but w/out any`&`or`&&`attached
	- when used here it gives the basic type `int`, then we give it a `&` again, all these is to avoid ex:
```cpp
template<typename T>
void f(T& x);

// client case a:
int a = 5; // T=int, so T& is int&
f(a); // T deduced as int, this is OK
// client case b:
int a = 5;
int& ref = a;
f(ref); // T deduced as int&, so `T&` becomes `int& &` — ❌ invalid syntax
```
###### ### Reference collapsing rules (C++11 and later)
Whenever a reference type appears within another reference, it collapses into:

| Combination | Collapses to | Meaning          |
| ----------- | ------------ | ---------------- |
| `& &`       | `&`          | lvalue dominates |
| `& &&`      | `&`          | lvalue dominates |
| `&& &`      | `&`          | lvalue dominates |
| `&& &&`     | `&&`         | stays rvalue ref |