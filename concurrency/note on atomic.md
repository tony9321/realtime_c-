#Voloridge 
lock free thread safe operations under `std::atomic`
```cpp
#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> counter=0;

void increment()
{
    for(int i=0;i<1000;i++)
        counter++; //atomic increment, shared int 
}

int main()
{
    std::thread t1(increment), t2(increment);
    t1.join();
    t2.join();
    std::cout << counter << "\n"; // guaranteed 2000
}
```
without `std::atomic`, there can be a race condition since both threads can read and write at the same time.
`std::atomic`ensures read-modify-write sequence of `counter++`happens at one atomic operation
### Mechanics
compiler calls `counter.fetch_add(1, std::memory_order_seq_cst);`a single atomic instruction at CPU level so we don't need `std::lock_gaurd`or`std::mutex` 
`std::mutex`provides mutual exclusion
### conditional optimization
`counter.fetch_add(1, std::memory_order_relaxed);`guarantees the atomicity of the increment, but not the ordering of unrelated memory operations, used for high-performance counters where strict ordering doesn’t matter.
[atomic.cpp](atomic.cpp)
#### Note on python atomic(not real concurrent)
a concept achieved by `GIL(Global Interpreter Lock)`makes some simple operations atomic
```python
x = 0
x += 1  # atomic at the bytecode level
```
However, multistep operation like `x+=1`aren't always atomic because they can release the `GIL`or involve multiple byte code instructions
##### Real concurrent in python(real atomic behavior)
```python
from threading import lock
```
or higher level thread safe obj:
```python
from queue import Queue
```
There's also `multiprocessing.Value`or `multiprocessing.Array`for atomic-like shared data btw processes 
## 

| Concept    | C++                                                                           | Python                                                                                   |
| ---------- | ----------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------- |
| **Atomic** | Explicit type: `std::atomic<T>`. Lock free, low-level, controls memory order. | Implicit via GIL; limited. Use `threading.Lock` or `multiprocessing` for real atomicity. |



