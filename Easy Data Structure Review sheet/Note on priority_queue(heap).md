#Voloridge 
use in Dijkstra
complete bin tree in arr: good cache locality
```csharp
Index: 0      1      2      3      4      5      6
       [root] [L]    [R]    [LL]   [LR]   [RL]   [RR]
parent(i) = (i - 1) / 2
left(i)   = 2*i + 1
right(i)  = 2*i + 2
```
## insert(push)
O(log n)
Append at end of vec
Bubble up til heap property restored
## pop
O(log n)
Swap root w/ last element
Remove last (former root)
Heapify down restore heap property
## Seek
O(1)
## make heap
Floyd’s trick->O(n)
