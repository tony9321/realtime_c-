compressed flat prefix array encodes the idea of `seg tree` w/ bit operations
specialized seg tree for prefix-type associative ops
- prefix queries (`sum[0..i]`) in `O(log n)`
- point updates (`add k to arr[i]`) in `O(log n)`
- cache friendly
- branch predictable
- can only do prefix aggregates (usually sum)
- stores partial prefix sums:  
    index `i` covers the range `(i − LSB(i) + 1 … i)` `LSB` is the least significant set bit
    No explicit tree nodes, just binary decomposition of indices