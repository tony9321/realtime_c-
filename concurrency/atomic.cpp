#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> counter=0;

void increment()
{
    for(int i=0;i<1000;i++)
        counter++; //atomic increment
}

int main()
{
    std::thread t1(increment), t2(increment);
    t1.join();
    t2.join();
    std::cout << counter << "\n"; // guaranteed 2000
}
