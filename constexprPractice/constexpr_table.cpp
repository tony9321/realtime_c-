#include<array>
#include <iostream>

// compile time table size
constexpr size_t TABLE_SIZE=10;

// build the lookup table at compile time
constexpr std::array<double, TABLE_SIZE> build_square_table()
{
    std::array<double, TABLE_SIZE> table{};
    for(size_t i=0;i<TABLE_SIZE;i++)
        table[i]=static_cast<double>(i*i);
    return table;
}

//Global constexpr table
constexpr auto square_table=build_square_table();

int main()
{
    // Runtime: O(1) lookup, no computation
    for (size_t i = 0; i < TABLE_SIZE; ++i)
        std::cout << "square[" << i << "] = " << square_table[i] << "\n";
}
