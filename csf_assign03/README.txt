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

To test our theories, we have conducted 24 experiments with the assumed cache size of 65536 bytes. Below are our trial implementations
and its respective total cycle numbers:

4096    1       16  no-write-allocate   write-through   lru:    11,942,869

64      64      16  no-write-allocate   write-through   lru:    11,655,144

1       4096    16  no-write-allocate   write-through   lru:    11,653,544

4096    1       16  no-write-allocate   write-through   fifo:   11,942,869

64      64      16  no-write-allocate   write-through   fifo:   11,759,307

1       4096    16  no-write-allocate   write-through   fifo:   11,811,838

256    1        256 no-write-allocate   write-through   lru:    30,594,363

16     16       256  no-write-allocate   write-through  lru:    15,145,781

1      256      256  no-write-allocate   write-through  lru:    15,068,994

256    1       256  no-write-allocate   write-through   fifo:   30,594,363

16      16      256  no-write-allocate   write-through  fifo:   19,373,744

1       256    256  no-write-allocate   write-through  fifo:    23,122,676

4096    1       16  write-allocate       write-back     lru:     6,166,800

64      64      16  write-allocate       write-back     lru:     5,580,000

1       4096    16  write-allocate       write-back     lru:     5,566,800

4096    1       16  write-allocate      write-back     fifo:     6,166,800

64      64      16  write-allocate       write-back     fifo:    5,675,200

1       4096    16  write-allocate      write-back     fifo:     5,664,000

256    1        256 write-allocate      write-back     lru:     29,862,800

16     16       256  write-allocate     write-back     lru:     10,739,600

1      256      256  write-allocate     write-back     lru:     10,624,400

256    1       256  write-allocate      write-back     fifo:    29,862,800

16      16      256  write-allocate     write-back     fifo:    12,365,200

1       256    256  write-allocate      write-back     fifo:    11,974,800

From the data above, we find the most effective cache is with small block, lru, write allocate, write back, fully  associative. 
Meanwhile, we observe that in general, the total cycle numbers follow the pattern below: 
1. fifo > lru ; 
As we explained in our hypothesis I earlier, using a lru eviction helps to keep those frequently-acessed blocks in the cache and therefore is 
more likely to have a higher hit rate than the fifo implementation. This helps reduce the total cycle number because load/stores from/to cache
requires only 1 processor cycle, but load/stores from/to memory takes as large as 100 processor cycles. 

2. NWA + WT > WA + WB ; 
NWA + WT accesses memory every time, which takes 100 processor cycles each time. Whereas WA + WB has a temporary intermediate 
space cache where it can access data with only 1 processor cycle, greatly enhancing its effectiveness and reducing the total cycles.

3. Direct Map > Set Associative > Fully Associative ; 
Fully associative uses cache to its maximum capacity and only needs replacement when the cache is completely filled. Therefore it has the lowest miss
rate, contributing to the lowest cycle count. Meanwhile, Direct Map would require a highly replacement frequency than a set associative, leading to higher miss rate
and more total cycle used. 

An interesting observation, however, is that fully associative, though being the most effective method using our cycle count, nevertheless takes the longest runtime. As
we have already suggested in our previous hypothesis II, this may be due to the potential updating time penalty since every access or load has to iterate over the entire cache to
update access/load ts. This takes significantly more time but is not considered in our cycle count. 

4. large block > small block

Since load/store processor cycle is performed on 4 byte units, larger block means more processor cycle when loading/storing a block, therefore resulting in larger cycle counts in every
load/store from/to memory. 

