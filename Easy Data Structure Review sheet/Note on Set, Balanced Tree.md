#Voloridge 
## `std::set`: 
sorted(stable)
uses red black tree
`lower_bound`
## red black tree(self-balancing bst)
nodes on heap
- key
- lptr, rptr
- color bit
tree rotations, recoloring
height: log(n)
Memory Not contiguous
### Coloring:
Root is black
No consecutive red
Every path from root to NIL has same # black 
All leaves (NIL ptr) are black
Every path from a node to leaf has same # black (black height)
color rules instead of strict heights like AVL
### Rotation
O(log n), each O(1)
Insert new node N:
#### left rotation:
```powershell
    x         y
     \       / \
      y  →  x   C
     / \     \
    B   C     B
```
#### right rotation:
```powershell
      y     x
     /     / \
    x   → A   y
   / \       /
  A   B     B
```
#### Case Uncle U is red: no rotation, just recolor
violates red-red 
```powershell
      G (black)
     / \
   P(r) U(r)
   /
 N(r)
```
Fix: 
```powershell
     G(r)   <-- temporarily red
    /   \
 P(b)   U(b)
  /
 N(r)
```
then repaint root to black:
```powershell
     G(b)
    /   \
 P(b)   U(b)
  /
 N(r)
```
#### Case Uncle U is BLACK: N is inner child (Triangle Case)
Example (Left-Right Case):
```powershell
      G
     /
   P
     \
      N   <-- inserted, red
```
Fix:
left rotation:
```powershell
      G
     /
    N
   /
  P
```
right rotation:
```powershell
   N
  / \
 P   G
```
then recolor:
```powershell
      P(b)
     /   \
   N(r)   G(r)
```

| insert  | O(log n) | tree walk, rotations |
| ------- | -------- | -------------------- |
| erase   | O(log n) | same as insert       |
| find    | O(log n) | bin search           |
| iterate | O(n)     | in order traversal   |

## `std::unordered_set`:
hash table with chaining
O(1) lookup

| insert    | O(1) avg      | bucket insert              |
| --------- | ------------- | -------------------------- |
| erase     | O(1) avg      | find in bucket then unlink |
| find      | O(1) avg      | hash, bucket search        |
| iteration | O(n) unsorted | scanning buckets           |
## AVL Tree
same way to rotate as red black tree
rotate when subtree height diff> 1
closer to perfect balance bc/ of stricter rule
- LL: right rotation
- RR: left rotation
- LR: left rotate child, right rotate parent
- RL: right rotate child, left rotate parent


## Conclusion
best set: Abseil flat_hash_set (contiguous open addressing)


> If I really care about performance, I’d use a flat open-addressing hash set like absl::flat_hash_set or Folly’s F14, which store elements contiguously and use SIMD to probe multiple slots at once — much more cache-efficient
   