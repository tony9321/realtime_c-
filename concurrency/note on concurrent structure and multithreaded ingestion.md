#Voloridge 
### Bounded v.s. Unbounded
Prefer bounded stucture
#### Unbounded: 
no sz limit, producers can keep pushing data 
If consumers fall behind, the queue grows until memory runs out
#### Bounded:
ex: The queue has a max sz (like 1,000 messages)
If it’s full, producers must wait until consumers process some items.  

### High Through Put Event Queue
thread safe structure that buffers events between producers and consumers
`Throughput=Total events processed/Total Time`
#### Implementation:
##### Ring Buffer(Circular Buffer, Single Producer, Single Consumer)
Bounded queue design
fixed size arr that wraps around
- No dynamic allocation
- cache friendly
- Easy to make `lock free` (each producer/consumer only touches its own pointer)
note: CAS (Compare-And-Swap): atomic instruction that makes lock-free programming possible
##### Avoid Locks when possible
lock free, wait free queues(avoid OS context switches)
When must lock, lock smaller critical sections
##### Batch Operations
Instead of processing one event at a time, dequeue `batches`of items
- better amortized time comlexity
- better cache usage
##### Cache Warming
multicore: `sched_setaffinity`: Pin producer, consumer threads to specific CPU cores(keeping cache warm)
Keep queue memory local to those cores
```cpp
std::vector<Event> batch;
while (queue.try_pop(batch, N)) {
    process_batch(batch);
}
```
#### Example:
###### Single-producer single-consumer ring buffer:
```cpp
std::array<Event, N> buf;
std::atomic<size_t> head = 0, tail = 0;
```
- Producer writes to `tail`, increments atomically
- Consumer reads from `head
- Modulo by N wraps around: circular buffer
- Lock-free
- O(1)
- cache friendly
###### multiple producers consumers
- TBB concurrent_bounded_queue
- Moodycamel’s ConcurrentQueue
- Disruptor pattern
###### Real market data pipeline:
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

> For performance, I’m mindful of cache alignment and SIMD vectorization when processing time-series data.  
> For ingestion, I’d use a multithreaded producer–consumer setup with bounded queues for controlled throughput.  
> On the quant side, I apply vectorized math — covariance matrices, rolling filters — for efficient signal generation.  
> And I always design with modular abstractions so the research code stays clean and maintainable.
.  For market data ingestion, I’d use a bounded lock-free ring buffer with cache-line alignment         and per-thread batching to sustain millions of events per second while avoiding false sharing.
   Event queue throughput is the system’s ability to move and process millions of events per second. You optimize it using bounded, lock-free ring buffers, cache alignment, batching, and per-core pinning. That keeps latency predictable and avoids contention under market spikes. “CAS is the atomic primitive that lets us implement lock-free structures.  
   Lock-free algorithms rely on CAS loops instead of mutexes, which gives low-latency and high parallelism — perfect for high-throughput systems.  
   The subtle danger is the ABA problem, where a value changes A→B→A and CAS incorrectly succeeds; we avoid it with tagged pointers, hazard pointers, or version counters.”
