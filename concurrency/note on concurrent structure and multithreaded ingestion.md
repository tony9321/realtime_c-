### Concurrent Structure:
`intel tbb concurrent map`
### Bounded v.s. Unbounded
Prefer bounded stucture
#### Unbounded: 
no sz limit, producers can keep pushing data 
If consumers fall behind, the queue grows until memory runs out
#### Bounded:
ex: The queue has a max sz (like 1,000 messages)
If it’s full, producers must wait until consumers process some items.  

### Ring Buffer(Circular Buffer)
Bounded queue desgin
fixed size arr that wraps around
- No dynamic allocation
- cache friendly
- Easy to make `lock-free` (each producer/consumer only touches its own pointer)
### Example:
Real market data pipeline:
`Producer threads`: receive raw messages (ticks, quotes)
`Consumer threads`: parse, clean, and write to db
They talk thru `Bounded Queue` like `Ring buffer`
```cpp
std::queue<Tick> q;
std::mutex m;
std::condition_variable cv;
```
prevents memory blow-up, producers wait if it’s full, consumers wait if it’s empty

Achieves:
- Throughput control: no unbounded memory
- Parallelism: one thread writes, others process
- Low latency: ingestion of data feeds
some lock free structure that's even faster:
`boost::lockfree::queue` or `folly::ProducerConsumerQueue` 



> “For performance, I’m mindful of cache alignment and SIMD vectorization when processing time-series data.  
> For ingestion, I’d use a multithreaded producer–consumer setup with bounded queues for controlled throughput.  
> On the quant side, I apply vectorized math — covariance matrices, rolling filters — for efficient signal generation.  
> And I always design with modular abstractions so the research code stays clean and maintainable.”