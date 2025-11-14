#Voloridge 
multi-way balanced search trees to min disk I/O for giant datasets(disk block is expensive)
- typical tree height: 2~4
- O(log n) search, insert, delete
- ordered iteration
- not for interval aggregates like BIT, Seg tree
- range queries in a sorted key space
For:
filesystems(Linux filesystems), databases(MySQL, Postgres indexes)