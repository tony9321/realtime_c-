#Voloridge 
less used in real world
Dynamic, binary tree
- Range queries: (like min/max/sum on `[l, r]`) `O(log n)`
- Point updates: (change a single element) or even range updates (like lazy propagation)  `O(log n)`
- Memory: `O(4n)`
- divides the range `[0, n−1]` recursively in halves.  Each node represents an interval, like `[0, 7] → [0, 3] + [4, 7]`.  Walk up or down the tree to aggregate ranges