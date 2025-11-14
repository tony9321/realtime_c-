#Voloridge 
Order book lookups (price → level)
## Open addressing 
- All entires live in arr, if bucket is full probe the next empty one(better cache locality)
- ex: c++'s `robin_hood`, `flat_hash_map` and python's `dict`
- avoid heap allocation
- have clustering issue
### Linaer probing
index = (hash(key) + i)%n,  for i = 0, 1, 2, 3, ...
### Quadratic probing
index = (hash(key) + i^2)%n,  for i = 0, 1, 2, 3, ...
ex: 3, 4, 7, 4+3²=12→4, wraps around... if h=3
### Double hashing
index = (h1(key) + i * h2(key))%n
### Rehash
when load factor(# elements stored/# buckets)>0.7 to make it about 2 times bigger and a prime number to achieve avg O(1)

### python's dict
use two parallel arrays
1. Indices array: stores 1-byte "control codes" describing slot state (empty, occupied, dummy)
2. Entries array: stores the actual `(key, value)` pairs in contiguous memory
3. Collision Resolution: Open Addressing with Perturbation(`perturb` val scrambles the probe path)
4. cache friendly

## Chaining
Each bucket is a linked list: arr of ptrs scattered in memory
`std::unordered_map`
rehash when load factor 1.0–2.0

- Robin Hood hashing: evens out probe lengths by stealing closer slots
- Swiss Table / Abseil’s `flat_hash_map`: stores small control bytes for occupancy, enabling SIMD lookups (16 probes at once! cmp 16 control bytes in parallel) , store (k, v, staus)
- tiny ptr hash map, less ptr overhead and better cache locality, not just bound by load factor and n, and probing now

> I prefer modern flat maps like absl::flat_hash_map or folly::F14FastMap for performance-critical paths. They use open addressing with Robin Hood probing, control bytes, and SIMD to probe multiple slots at once. That yields much better cache locality and predictable performance under high load compared to traditional std::unordered_map