/*
 * functions and struct that simulates a cache and record its settings(such as fifo)
 * CSF Assignment 3
 * Richard Zhang
 * rzhang89@jhu.edu
 */
#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>

struct blockHeader {//save the information of a block including its tag, dirty bits, valid bit, and its block_num(the relative position)
    uint8_t valid_bits;
    uint8_t dirty_bits;
    uint32_t tag;
    uint32_t block_num;
};

//(a node of) circular doubly linked list used for optimization: a head pointer will always point to the oldest node in this list so
//the victim can be easily found
typedef struct list_struct {
    struct blockHeader *block;//the actual data that it represents
    struct list_struct *next, *prev;
} list_t;

struct Cache{
  //I save each block in form of "everything else"(header) + "block data".
  uint32_t block_header_size; //side of the header file that includes valid bit(1 or 0), order bits, and tag bits
  uint32_t block_size; //block_header_size + num_of_bytes

  uint32_t num_of_sets; //number of sets
  uint32_t num_of_blocks; //number of blocks in each set
  uint32_t num_of_bytes; //number of bytes in each block

  uint32_t offset_bits; //size of offset(in bits) (log2 of block size)
  uint32_t index_bits; // size of index (log2 of number of sets)
  uint32_t tag_bits; //size of tag

  uint32_t loads; //number of loads
  uint32_t stores; //number of stores
  uint32_t load_hits; //number of load hits
  uint32_t load_misses; //number of load misses
  uint32_t store_hits; //number of store hits
  uint32_t store_misses; //number of store misses
  uint32_t cycles; //total number of cycles

  char miss_strategy; //miss strategy, 'w' for write allocate, 'n' for not
  char hit_strategy; //hit strategy, 't' for write through and 'b' for write back
  char replacement; //replacement strategy, 'l' for lru and 'f' for fifo
  char *data; //all data that's stored in this cache

  list_t* block_list; //the list that include a bunch of nodes that each represent a block(these nodes form multiple doubly linked lists, 1 list per set)
  list_t** set_head_list; //the array that includes the pointers to the worst one(least accessed or first in) of each set

};

// initiate a cache
// parameters are the pointer to a cache, the number of sets in the cache, number of blocks in each set, number of bytes in each block, its miss_strategy(write allocate or not)
// hit strategy(write through or write back), and replacement strategy(lru or fifo)
void initiate_cache(struct Cache *cache, int num_sets, int num_blocks, int num_bytes, char* miss_strategy, char* hit_strategy, char* replacement);

//calculate the size of block header(the space it occupies in the cache data)
//return its size in bits
//tag length is the size of the tag, count_bits is the ordering bits' length, type indicates fifo or lru
int calculate_block_header_size(int tag_length, int count_bits, char type);

//load according to an instruction
//cache is the cache we are working with, command is the 32bit input that we have to interpret and act accordingly
void load(struct Cache* cache, uint32_t command);

//store according to an instruction
//cache is the cache we are working with, command is the 32bit input that we have to interpret and act accordingly
void store(struct Cache* cache, uint32_t command);

//print the final result of the run based on the cache
void print_results(struct Cache* cache);

//frees all memory allocations in the cache
void free_cache(struct Cache* cache);

//decode the command(32 bit input) based on the information given in the cache(such as offset size, index size, and tag size)
//put the calculated tag and index into tag and index
void decode_command(struct Cache* cache,uint32_t command, uint32_t* tag, uint32_t* index);

//determines if a given tag and index combinition hits in the cache, return where this tag is if found, return -1 for no hit
int32_t is_hit(struct Cache* cache, uint32_t set_index, uint32_t tag);

//get the tag of the num-th(start with 0) block of the set-th set(start with 0) from the cache, return is the tag
uint32_t get_tag(struct Cache* cache, uint32_t set_index, uint32_t block_num);

//set the tag of the num-th(start with 0) block of the set-th set(start with 0) from the cache to the value of tag, return void
void set_tag(struct Cache* cache, uint32_t set_index, uint32_t block_num, uint32_t tag);

//get the validity(0 or 1) of the num-th(start with 0) block of the set-th set(start with 0) from the cache, return is the validity
uint8_t get_valid(struct Cache* cache, uint32_t set_index, uint32_t block_num);

//set the validity of the num-th(start with 0) block of the set-th set(start with 0) from the cache to the value of valid, return void
void set_valid(struct Cache* cache, uint32_t set_index, uint32_t block_num, uint8_t valid);

//get the dirty bit(0 or 1) of the num-th(start with 0) block of the set-th set(start with 0) from the cache, return is the dirty bit
uint8_t get_dirty(struct Cache* cache, uint32_t set_index, uint32_t block_num);

//set the dirty bit of the num-th(start with 0) block of the set-th set(start with 0) from the cache to the value of dirty, return void
void set_dirty(struct Cache* cache, uint32_t set_index, uint32_t block_num, uint8_t dirty);

//put a tag into the cache in the set's position, type identifies if it is a load or store(prior will "clean" the
//dirty bit while the latter will make a bit dirty)
void put_in(struct Cache* cache, uint32_t set_index, uint32_t block_num, uint32_t tag, char type);

//find a victim block from a set(indicated by set_index) that will be sacrificed, return is the index of this victim relative to the start of the set(start with 0)
uint32_t find_victim(struct Cache* cache, uint32_t set_index);

//initiate the block list
void initiate_block_list(struct Cache* cache);

//move a block(indicated by block_num) of a given set(indicated by set_index) to the tail of the linkedlist
//since we have a circular array, tail here means 1 before where the set pointer points to
void move_to_tail(struct Cache* cache, uint32_t set_index, uint32_t block_num);

// initiate_block_num, setting each block's blocknum to their ordering in the appropriate set 
void initiate_block_num(struct Cache* cache);

#endif
