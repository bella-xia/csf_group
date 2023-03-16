Team members: Hanbei Zhou, Bella Xia

We collectively worked on Makefile and main.cpp.

Best Cache:

(1) Total miss penalty. If a certain blocks of address is frequently accessed, using least-recently-used stratigy will insure a 
better perforance. Every access updates the access time of the cooresponding block. Therefore, the more frequently accessed block 
is more likely to be accessed recently and therefore less likely to be replaced. On the other hand, the access frequency does not 
correlate the loading orders. Thus, using a lru eviction helps to keep those frequently-acessed blocks in the cache and therefore is 
more likely to have a higher hit rate than the fifo implementation. Since every miss has a miss penalty, which lowers the runtime 
efficiency, the lru would have less misses and a higher efficiency than fifo.
(2) Updating time penalty. Consider two caches with the same number of slots; one cache has a larger size than the other and therefore 
has more blocks in each slot. Updating the access time or load time requires iterating over the whole slot, so the larger the cache is, 
the more time penalty it will cause for each update. When implementing with lru strategy, every access (both hit and miss) updates 
the access time of all blocks in the slot; while when implementing with fifo strategy, every miss updates the access time. This 
means that the lru strategy will have significantly more updates than fifo and therefore a larger time penalty.

Therefore, the time penalty caused by updating access time and the time penlaty caused by misses need to be weighed against each
other. The larger the cache is, the more updating-time-penalty is; the more repetition in data set and the larger each miss pentalty 
is, the more total-miss-penalty is.

