#Voloridge 
keeps critical section(actions that have to happen together) safe
```cpp
#include <mutex>
#include <thread>
#include <iostream>
#include <vector>

std::vector<int> v;
std::mutex m;

void add_to_vector(int x) {
    std::lock_guard<std::mutex> g(m); // lock acquired
    v.push_back(x);                   // modifies shared vector
}                                     // lock released here
```
`v.push_back(x)`isn't atomic so if two threads call it without a lock, it will crash
`std::mutex`ensures only one thread can enter that region at once
#### `std::atomic` v.s. `std::mutex`

| Use Case                                              | Tool          | Example          |
| ----------------------------------------------------- | ------------- | ---------------- |
| Single variable updates (counters, flags)             | `std::atomic` | `counter++`      |
| Multiple related operations or shared data structures | `std::mutex`  | `v.push_back(x)` |
#### Example:
This code breaks:
```cpp
struct Account { int balance; };
std::atomic<int> total; // total bank balance (for sanity checking)
Account A{100}, B{100};

void transfer(Account& from, Account& to, int amt) {
    from.balance -= amt;
    to.balance += amt;
}
```
Fix:
```cpp
std::mutex m;
void transfer(Account& from, Account& to, int amt) {
    std::lock_guard<std::mutex> g(m);
    from.balance -= amt;
    to.balance += amt;
}
```
### Dead Lock
multiple threads hold locks other one needs, so neither can proceed
```cpp
std::mutex m1, m2;

void task1() {
    std::lock_guard<std::mutex> g1(m1);
    std::lock_guard<std::mutex> g2(m2); // waits for m2
}

void task2() {
    std::lock_guard<std::mutex> g1(m2);
    std::lock_guard<std::mutex> g2(m1); // waits for m1
}
```
`task1` locks `m1` first and `task2` locks `m2` first, both are stuck forever waiting on each other
#### How to avoid dead lock
- lock in same order across all threads
- scoped lock(lock in consistent order) in cpp17
```cpp
std::scoped_lock g(m1, m2);
```
- keep critical section small
- 