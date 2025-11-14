#Voloridge 
## CPU Pipeline
CPU doesn't execute one instruction at a time, it pipelines them like an assembly line
1. `Fetch` the next instruction from mem
2. `Decode`operation: add, load, branch
3. `Execute`arithmetic or mem. operation
4. `Write back`res to register
## Branch prediction
however, branching breaks the flow when CPU guess the wrong branch(`if-else`)
So tight loops w/ predictable branches are faster
```cpp
int sum = 0;
for (int i = 0; i < 1'000'000; ++i) 
    if (i % 2 == 0)   // predictable branch
        sum += i;
    else sum -= i;
```
Fast w/ obvious branch pattern: true, false, true, false... so CPU can work ahead of time and make the right guess
However with this code:
```cpp
int sum = 0;
for (int i = 0; i < 1'000'000; ++i) 
    if (rand() % 2 == 0)   // unpredictable branch
        sum += i;
    else sum -= i;
```
This is unpredictable: can only be right half of the time, so the other half of the time that's wrong need to be flushed out of the pipeline
## Alignment
CPU likes to read memory at addresses that match data size
ex: An `int` (4 bytes) might need to start at an address multiple of 4
### struct layout
c++ insert padding for alignment
```cpp
struct A {
    char a;   // 1 byte
    int b;    // 4 bytes, but starts at offset 4, not 1
};
```
Total size becomes 8 bytes, not 5. Compilers insert “padding” bytes for alignment
## Cache
Cache lines is mostly 64 bytes(for spatial locality)
```cpp
int a[1000000];
for (int i = 0; i < 1000000; ++i)
    a[i] *= 2;
```
Each int is 4 bytes, so a 64-byte cache line contains 16 ints
However:
```cpp
for (int i = 0; i < 1000000; i += 64)
    a[i] *= 2;
```
touch only one int from every 64-byte line.
Each access pulls in a whole cache line but uses just 4 bytes, 1/16th hit
### False sharing btw/ multithread
Two threads updating diff vars that live on the same cache line will fight each other
```cpp
struct alignas(64) Counter {
    std::atomic<int> x;
};
Counter c1, c2; // each on its own cache line(separate 64byte boundary)
```
w/out `alignas(64)`, both `c1` and `c2` might sit in one line. When thread 1 updates `c1.x`, it invalidates thread 2’s cache line for `c2.x`, causing massive slowdowns — called `false sharing`

`Temporal locality` is when I use this once I might use it again soon

## SIMD(Single Instruction, Multiple Data)
Relying more on compiler instead of our code
CPU instructions that operate on many numbers at once (like add 4 floats at once)
`_mm_add_ps` or compiler vectorization(`-O3, -march=native`)
```cpp
// Instead of looping over one price at a time:
for (int i = 0; i < N; i++)
    price[i] += drift;
// SIMD can update 4–8 prices per instruction.
```
ex: When you process millions of price ticks or run Monte Carlo simulations, SIMD boosts throughput by 4–10×
