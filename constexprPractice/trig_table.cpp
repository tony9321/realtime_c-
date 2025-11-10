#include <array>
#include <iostream>
#include <cmath>

class TrigTable
{
public:
    static constexpr size_t SIZE=10;

    // build the sine lookup table at compile time
    inline static constexpr std::array<double, SIZE> sin_values=[]
    {
        std::array<double, SIZE> table{};
        for(size_t i=0;i<SIZE;i++)
            table[i]=std::sin(i*0.1);
        return table;
    }(); // immediately invoked lambda
};

int main()
{
    //access precomputed sine values instantly
    for(size_t i=0;i<TrigTable::SIZE;i++)
    {
         std::cout << "sin(" << i * 0.1 << ") = " << TrigTable::sin_values[i] << "\n";
    }
}
