Richard Zhang work individually in this project

Some difficulties and explanation of my implementation:

Since it wasn't said that we will have a time limit for this project, I initially implemented by having all data saved in one array of chars. Then 
I represented validity, dirtybits, data, and tags(initially I also saved order bits) in form of bits. This implementation save a lot of space
but was incredibly slow since I have to do many bitwise shifts to get the data. To meet the time requirement, I largely altered my implmentation: I
changed the data from bitwise representation to bytewise representation and put structs into it. I could have removed this data and replace it with multiple levels of structs,
but it is too time consuming to startover and redo everything. To track the order of elements in each set, I created a linkedlist for each set and have a pointer pointing 
to the next victim. This can largely increase the speed of finding the victim and removing the victim(of course, lru load is still linear time). I included a pointer
to the actual block(in data) in each node of the linkedlist and the data includes the actual information of the block.(I know that it looks weird, but I am doing this so that
my previous codes can be reused)

experiments:

In each experiment, there are 1024 blocks each with 16 bytes. Each valid "hit strategy-miss strategy-replacement strategy" combinition 
is tested with 1, 8, 32, 512, and 1024 blocks per set. This is done because the more blocks there are in each set, the more complicated 
the hardware will get. In other words, the block per set is also a part of the assessing criteria. I use gcc.trace as my sample data source 
since it is pretty large and can simulate real world scenarios.

below are the experiment results:
under setting, w means write allocate, n means no write allocate; b means write back and t means write through; f means fifo and l means lru

Total loads: 318197
Total stores: 197486
Load hits: 312238
Load misses: 5959
Store hits: 187502
Store misses: 9984
Total cycles: 11127283
Block per set: 1
Hit rate: 0.969084
Setting: w, b, f


Total loads: 318197
Total stores: 197486
Load hits: 314249
Load misses: 3948
Store hits: 188090
Store misses: 9396
Total cycles: 9781683
Block per set: 8
Hit rate: 0.974124
Setting: w, b, f


Total loads: 318197
Total stores: 197486
Load hits: 314293
Load misses: 3904
Store hits: 188081
Store misses: 9405
Total cycles: 9765683
Block per set: 32
Hit rate: 0.974191
Setting: w, b, f


Total loads: 318197
Total stores: 197486
Load hits: 314317
Load misses: 3880
Store hits: 188115
Store misses: 9371
Total cycles: 9731683
Block per set: 512
Hit rate: 0.974304
Setting: w, b, f


Total loads: 318197
Total stores: 197486
Load hits: 314311
Load misses: 3886
Store hits: 188117
Store misses: 9369
Total cycles: 9734883
Block per set: 1024
Hit rate: 0.974296
Setting: w, b, f


Total loads: 318197
Total stores: 197486
Load hits: 312238
Load misses: 5959
Store hits: 187502
Store misses: 9984
Total cycles: 11127283
Block per set: 1
Hit rate: 0.969084
Setting: w, b, l


Total loads: 318197
Total stores: 197486
Load hits: 314908
Load misses: 3289
Store hits: 188278
Store misses: 9208
Total cycles: 9277683
Block per set: 8
Hit rate: 0.975766
Setting: w, b, l


Total loads: 318197
Total stores: 197486
Load hits: 314946
Load misses: 3251
Store hits: 188273
Store misses: 9213
Total cycles: 9268483
Block per set: 32
Hit rate: 0.975830
Setting: w, b, l


Total loads: 318197
Total stores: 197486
Load hits: 314972
Load misses: 3225
Store hits: 188306
Store misses: 9180
Total cycles: 9223683
Block per set: 512
Hit rate: 0.975945
Setting: w, b, l


Total loads: 318197
Total stores: 197486
Load hits: 314973
Load misses: 3224
Store hits: 188300
Store misses: 9186
Total cycles: 9226883
Block per set: 1024
Hit rate: 0.975935
Setting: w, b, l


Total loads: 318197
Total stores: 197486
Load hits: 312238
Load misses: 5959
Store hits: 187502
Store misses: 9984
Total cycles: 25643083
Block per set: 1
Hit rate: 0.969084
Setting: w, t, f


Total loads: 318197
Total stores: 197486
Load hits: 314249
Load misses: 3948
Store hits: 188090
Store misses: 9396
Total cycles: 24662283
Block per set: 8
Hit rate: 0.974124
Setting: w, t, f


Total loads: 318197
Total stores: 197486
Load hits: 314293
Load misses: 3904
Store hits: 188081
Store misses: 9405
Total cycles: 24647383
Block per set: 32
Hit rate: 0.974191
Setting: w, t, f


Total loads: 318197
Total stores: 197486
Load hits: 314317
Load misses: 3880
Store hits: 188115
Store misses: 9371
Total cycles: 24627583
Block per set: 512
Hit rate: 0.974304
Setting: w, t, f


Total loads: 318197
Total stores: 197486
Load hits: 314311
Load misses: 3886
Store hits: 188117
Store misses: 9369
Total cycles: 24629383
Block per set: 1024
Hit rate: 0.974296
Setting: w, t, f


Total loads: 318197
Total stores: 197486
Load hits: 312238
Load misses: 5959
Store hits: 187502
Store misses: 9984
Total cycles: 25643083
Block per set: 1
Hit rate: 0.969084
Setting: w, t, l


Total loads: 318197
Total stores: 197486
Load hits: 314908
Load misses: 3289
Store hits: 188278
Store misses: 9208
Total cycles: 24342283
Block per set: 8
Hit rate: 0.975766
Setting: w, t, l


Total loads: 318197
Total stores: 197486
Load hits: 314946
Load misses: 3251
Store hits: 188273
Store misses: 9213
Total cycles: 24328583
Block per set: 32
Hit rate: 0.975830
Setting: w, t, l


Total loads: 318197
Total stores: 197486
Load hits: 314972
Load misses: 3225
Store hits: 188306
Store misses: 9180
Total cycles: 24308283
Block per set: 512
Hit rate: 0.975945
Setting: w, t, l


Total loads: 318197
Total stores: 197486
Load hits: 314973
Load misses: 3224
Store hits: 188300
Store misses: 9186
Total cycles: 24309683
Block per set: 1024
Hit rate: 0.975935
Setting: w, t, l

Total loads: 318197
Total stores: 197486
Load hits: 309108
Load misses: 9089
Store hits: 162581
Store misses: 34905
Total cycles: 23864978
Block per set: 1
Hit rate: 0.914688
Setting: n, t, f


Total loads: 318197
Total stores: 197486
Load hits: 311124
Load misses: 7073
Store hits: 164120
Store misses: 33366
Total cycles: 23060117
Block per set: 8
Hit rate: 0.921582
Setting: n, t, f


Total loads: 318197
Total stores: 197486
Load hits: 311145
Load misses: 7052
Store hits: 164314
Store misses: 33172
Total cycles: 23051911
Block per set: 32
Hit rate: 0.921999
Setting: n, t, f


Total loads: 318197
Total stores: 197486
Load hits: 311170
Load misses: 7027
Store hits: 164350
Store misses: 33136
Total cycles: 23041947
Block per set: 512
Hit rate: 0.922117
Setting: n, t, f


Total loads: 318197
Total stores: 197486
Load hits: 311160
Load misses: 7037
Store hits: 164196
Store misses: 33290
Total cycles: 23045793
Block per set: 1024
Hit rate: 0.921799
Setting: n, t, f

Total loads: 318197
Total stores: 197486
Load hits: 309108
Load misses: 9089
Store hits: 162581
Store misses: 34905
Total cycles: 23864978
Block per set: 1
Hit rate: 0.914688
Setting: n, t, l


Total loads: 318197
Total stores: 197486
Load hits: 311693
Load misses: 6504
Store hits: 164933
Store misses: 32553
Total cycles: 22833330
Block per set: 8
Hit rate: 0.924262
Setting: n, t, l


Total loads: 318197
Total stores: 197486
Load hits: 311732
Load misses: 6465
Store hits: 164772
Store misses: 32714
Total cycles: 22817569
Block per set: 32
Hit rate: 0.924025
Setting: n, t, l


Total loads: 318197
Total stores: 197486
Load hits: 311735
Load misses: 6462
Store hits: 164779
Store misses: 32707
Total cycles: 22816376
Block per set: 512
Hit rate: 0.924044
Setting: n, t, l


Total loads: 318197
Total stores: 197486
Load hits: 311723
Load misses: 6474
Store hits: 164778
Store misses: 32708
Total cycles: 22821175
Block per set: 1024
Hit rate: 0.924019
Setting: n, t, l

Interpretation:
Generally speaking, hit rate is maximized and total number of cycles are minimized when we have 2 sets(512 blocks per set). However, we don't see significant optimization 
in either hit rate or cycle count in the shift from 32 blocks per set to 512 blocks per set. Considering that it is generally much easier to implement 32 blocks 
per set, I suggest that it has the best overall effectiveness.

Among the 32 block per set results, lru always outperforms fifo in both cycle and hit rate, therefore I exclude all fifo experiment results from the list of 
possible best configuration. Similar case happens for no-write-allocate and I exclude it from my list as well.

The remaining two configurations, "w,b,l"(write allocate, write back, lru) and "w,t,l"(same, but change the middle one to write through) have roughly the same 
hit rate. However, "w,b,l" has a significantly smaller cycle count(9268483 vs 24328583), making it better than "w,t,l".

Therefore, according to my experiment results and analysis, the configuration with the best effectiveness is: 32 blocks per set, write allocate, write back, lru.