// This file is to practice RAII (Resource Acquisition Is Initialization): the constructor acquires a resource (start time), the destructor releases it (prints result).
// Notice how we don’t even need a method like start() or stop(). Just creating the object = start, destroying it = stop. That’s RAII.
#include <chrono>
#include <iostream>
#include <atomic>
class Timer
{
public:
    Timer(); // timer
    ~Timer(); // stop and print elasped]
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};
// constructor
Timer::Timer()
{
    start=std::chrono::high_resolution_clock::now();
}
// destructor
Timer::~Timer()
{
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    std::cout << "Elapsed: " <<duration << "ns\n";
};

int main()
{
    Timer t; // starts timing here

    // volatile is to tell compiler not to optimize this so it doesn't notice that this loop does nothing, but this the old way to do it 
    //for (volatile int i = 0; i < 1000000000; i++);
    
    // the new way to do it is inline with assembly
    // empty string, meaning we don’t emit any actual CPU instruction.
    // separates parts of the assembly template (inputs, outputs, clobbers)
    // tells the compiler: “assume memory could be changed.”
    // So, even though the assembly does nothing, the compiler must act as if memory might be touched, which forces it to keep the loop iterations intact.
    // only works on clang g++
    //for (int i = 0; i < 1000000000; i++) {
      //  asm volatile("" ::: "memory"); // prevents optimization
    //}

    // or c++20, portable on MSVC as well
    for (int i = 0; i < 1000000000; i++) {
        std::atomic_signal_fence(std::memory_order_seq_cst);
    }
}// destructor runs automatically here, printing elapsed time
