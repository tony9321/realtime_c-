#Voloridge 
prefix tree where each edge corresponds to a character and each node corresponds to a prefix
Advantqge:
- no hashing required
- Fast lookup
- Storing huge numbers of strings with shared prefixes

| Insert        | O(L)                      |
| ------------- | ------------------------- |
| Lookup        | O(L)                      |
| Prefix search | O(L + output_size)        |
| Space         | O(sum of lengths of keys) |
trie node:
- An array of children (size depends on alphabet)
- A bool indicates end of key
```cpp
map<char, TrieNode*> children
children: array of size 26
end_of_word: bool
```

### Better Performance Variant:
#### Radix Tree (Compressed Trie)
Instead of one character per node, compress chains
```cpp
C → A → T //becomes node labeled "CAT"
```
Shallower tree = faster
!! stores ticker
#### Patricia Trie (bitwise trie / crit-bit tree)
Each edge splits on **bit positions**, not characters
- IP routing (CIDR prefixes)
- Network device
- Low-level system libs
#### DAWG (Directed acyclic word graph)
compressed trie that merges equivalent subtrees
- Dictionary compression
- Spell checkers
- Word games
#### Ternary Search Tries
BST-style structure at each node:  
left < char < right

> Trie is a tree where each edge corresponds to a character or bit. Lookups are O(length of key) and prefix queries are extremely fast. It avoids hash collisions and compresses shared prefixes well. Variants like radix trees and Patricia tries improve memory locality and depth, and are used in kernels, network routing, and large symbol dictionaries. The main tradeoff is memory cost versus very fast structured lookups.