#Voloridge 
`std::condition_variable`
threads wait efficiently for something to change, instead of keep spinning in a loop
```cpp
#include <queue>
#include <mutex>
#include <condition_variable>

std::queue<int> q;
std::mutex m;
std::condition_variable cv;
```
- **Producer thread**:
    1. Lock mutex
    2. Adds items to `q`
    3. Calls`cv.notify_one()`to wake up a waiting consumer
- **Consumer thread**:
    1. Lock mutex
    2. Waits until`q`isn’t empty: `cv.wait(lock, []{ return !q.empty(); });`
    3. Pops item