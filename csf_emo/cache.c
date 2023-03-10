/*
 * functions and struct that simulates a cache and record its settings(such as fifo)
 * CSF Assignment 3
 * Richard Zhang
 * rzhang89@jhu.edu
 */
#include "cache.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

//initiate a cache
//parameters are the pointer to a cache, the number of sets in the cache, number of blocks in each set, number of bytes in each block, its miss_strategy(write allocate or not)
//hit strategy(write through or write back), and replacement strategy(lru or fifo)
void initiate_cache(struct Cache *cache, int num_sets, int num_blocks, int num_bytes, char* miss_strategy, char* hit_strategy, char* replacement) {
    cache->num_of_sets = num_sets;
    cache->num_of_blocks = num_blocks;
    cache->num_of_bytes = num_bytes;

    //set miss strategy, hit strategy, and replacement strategy based on the input given
    cache->miss_strategy = !strcmp("write-allocate",miss_strategy)? 'w':'n';
    cache->hit_strategy = !strcmp("write-through",hit_strategy)? 't':'b';
    cache->replacement = !strcmp("lru",replacement)? 'l':'f';

    //calculate the length of offset, index, and tag
    cache->offset_bits = (uint32_t)log2(num_bytes);
    cache->index_bits = (uint32_t)log2(num_sets);
    cache->tag_bits = 32 - cache->offset_bits - cache->index_bits;

    //calculate the block size
    cache->block_header_size = sizeof(struct blockHeader);
    cache->block_size = sizeof(struct blockHeader) + num_bytes;

    //allocate memory for all blocks
    cache->data = calloc(1,cache->block_size * num_blocks * num_sets);

    // use block list
    cache->block_list = malloc(sizeof(list_t) * num_sets *num_blocks);
    cache->set_head_list = malloc(sizeof(list_t) * num_sets);

    initiate_block_list(cache);
    initiate_block_num(cache);
}

//initiate_block_num, setting each block's blocknum to their ordering in the appropriate set 
void initiate_block_num(struct Cache* cache) {
    for(uint32_t i=0; i< cache->num_of_sets; i++) {
        for(uint32_t j=0; j<cache->num_of_blocks; j++) {
            uint64_t count = (uint64_t)((i* cache->num_of_blocks + j) * cache->block_size); //calculate where this block is
            struct blockHeader* block = (struct blockHeader*) (cache->data + count);
            block->block_num = j;

        }
    }
}

//initiate block_list
void initiate_block_list(struct Cache* cache) {
    for(uint32_t i = 0;i < cache->num_of_sets;i++) {//for every set
        char* set_h = cache->data + i * cache->num_of_blocks  * cache->block_size;//create a pointer that points to the start of this set(in data)
        list_t* list_h = cache->block_list + i * cache->num_of_blocks; //make list_h a pointer to the start of the set(in block list)
        list_h->block = (struct blockHeader*)set_h;//relate set_h and list_h->block, essentially saving the block address(in data) in the current block

        for(uint32_t j = 1;j < cache->num_of_blocks; j++) {//for every block in the set
            list_t* p_blk = list_h + j; //get the current node's(the node that represent the block) pointer

            //set its prev to be its previous one and its prev's next to be itself 
            p_blk->prev = list_h+j-1;
            (list_h+j-1)->next = p_blk;

            //relate each block representation in list to the actual block data in the array data
            char* blk = set_h + j*cache->block_size;
            p_blk->block = (struct blockHeader*)blk;
        }
        //this is a circular linked list, the last one and the first one are connected
        list_h->prev = list_h + cache->num_of_blocks -1;
        list_h->prev->next = list_h;

        //this linkedlist is done, pass it back to the appropriate set_head_list
        *(cache->set_head_list + i ) = (list_t*)list_h;
    }
}


//load according to an instruction
//cache is the cache we are working with, command is the 32bit input that we have to interpret and act accordingly
void load(struct Cache* cache, uint32_t command){
    cache->loads += 1; //increment number of loads
    uint32_t tag;
    uint32_t set_index;
    decode_command(cache,command,&tag,&set_index); //get the appropriate tag and index from the command

    int64_t block_num = is_hit(cache, set_index, tag);
    if(block_num != -1){ //in case of hit
        cache->load_hits += 1; //increment total number of hits
        cache->cycles += 1; //loading from cache takes one cycle

    } else {
        cache->load_misses += 1;
        // find block_num
        block_num = find_victim(cache, set_index);

        put_in(cache,set_index, block_num, tag,'l'); //put the tag into the set
        cache->cycles++; // read from cache
    }

    if(cache->replacement == 'l') {
        // move the node to tail(make it the newest one)
        move_to_tail(cache, set_index, block_num);
    }
}

//store according to an instruction
//cache is the cache we are working with, command is the 32bit input that we have to interpret and act accordingly
void store(struct Cache* cache, uint32_t command) {
    cache->stores += 1;
    uint32_t tag;
    uint32_t set_index;
    decode_command(cache,command,&tag,&set_index); //get the appropriate tag and index from the command
    int block_num = is_hit(cache, set_index, tag); //save where this hit occurs(-1 for no hit)

    if(block_num != -1){ //in case of hit
        cache->store_hits += 1; //increment total number of store-hits
        if (cache->hit_strategy != 't') {//for write back, we store in the cache and write to the memory later
            cache->cycles += 1;
            set_dirty(cache,set_index,block_num,1); //since we modifies this value, it is now dirty
        } else {
            cache->cycles += 1;
            cache->cycles += 100; //if it is write through, we write directly to the memory
        }
    } else { //in case of miss
        cache->store_misses += 1;

        if(cache->miss_strategy == 'n') { //if we have no write allocate, then don't do anything to the cache
           cache->cycles +=  100; //save directly to the memory(not through the cache, takes 100 cycles)
           return;
        }

        if(cache->miss_strategy == 'w') {
            block_num = find_victim(cache, set_index);
            put_in(cache,set_index, block_num, tag,'s'); //put the tag into the set
            cache->cycles++; //save to the cache takes 1 cycle
        }

    }

    if(cache->replacement == 'l') {
        // move the node to tail
        move_to_tail(cache, set_index, block_num);
    }
}

//move a block(indicated by block_num) of a given set(indicated by set_index) to the tail of the linkedlist
//since we have a circular array, tail here means 1 before where the set pointer points to
void move_to_tail(struct Cache* cache, uint32_t set_index, uint32_t block_num) {
    list_t* list_h = *((cache->set_head_list) + set_index); //find the head of the set(as a pointer to the starting block)
    list_t* cur = list_h; //initialize cur to point to the "head node"

    while( cur->block->block_num != block_num) //we find the node that we want to move to tail
        cur = cur->prev;

    if(cur == list_h) {//if the one we want to move to tail is the first one
        *((cache->set_head_list) + set_index) = list_h->next; //just move the "set head pointer" to the next one, so the current one is automatically the last one
        return;
    }

    if(cur->next == list_h)//if the one we want to move to tail is already in the tail, don't do anything
        return;

    //regular cases
    //remove cur from the linkedlist for now
    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;

    //put it back in as the one right before the head
    cur->next = list_h;
    cur->prev = list_h->prev;
    list_h->prev->next = cur;
    list_h->prev = cur;
}

//print the final result of the run based on the cache
void print_results(struct Cache* cache) {
    printf("Total loads: %d\n",cache->loads);
    printf("Total stores: %d\n",cache->stores);
    printf("Load hits: %d\n",cache->load_hits);
    printf("Load misses: %d\n",cache->load_misses);
    printf("Store hits: %d\n",cache->store_hits);
    printf("Store misses: %d\n",cache->store_misses);
    printf("Total cycles: %d\n",cache->cycles);

    //lines below are used to print the information needed to find the best configuration
    //printf("Block per set: %d\n",cache->num_of_blocks);
    //printf("Hit rate: %f\n",(cache->load_hits+cache->store_hits)/((float)cache->loads+cache->stores));
    //printf("Setting: %c, %c, %c\n\n\n",cache->miss_strategy,cache->hit_strategy,cache->replacement);
}

//frees all memory allocations in the cache
void free_cache(struct Cache* cache) {
    free(cache->data);
    free(cache->block_list);
    free(cache->set_head_list);
}

//decode the command(32 bit input) based on the information given in the cache(such as offset size, index size, and tag size)
//put the calculated tag and index into tag and index
void decode_command(struct Cache* cache,uint32_t command, uint32_t* tag, uint32_t* index) {
    command >>= cache->offset_bits; //remove the useless offset bits

    //(1 << index bits) - 1 creates a series of 1's that's the same length as index
    //command & this will remove all non-index values(the tag)
    *index = command & ((1<<cache->index_bits)-1);

    *tag = command >> cache->index_bits; //tag is just the remaining part removing the index
}

//determines if a given tag and index combinition hits in the cache, return where this tag is if found, return -1 for no hit
int32_t is_hit(struct Cache* cache, uint32_t set_index, uint32_t tag) {
    for(uint32_t i = 0;i < cache->num_of_blocks; i++) {
        if(!get_valid(cache,set_index,i)){
            continue; //skip the invalid blocks
        }
        if(get_tag(cache,set_index,i) == tag) {
            return i; //if we find the same tag, a hit is found
        }
    }
    return -1;
}

//get the tag of the num-th(start with 0) block of the set-th set(start with 0) from the cache, return is the tag
uint32_t get_tag(struct Cache* cache, uint32_t set_index, uint32_t block_num) {
    uint64_t count = (uint64_t)((set_index * cache->num_of_blocks + block_num) * cache->block_size); //calculate where this block is
    struct blockHeader* block = (struct blockHeader*) (cache->data + count);
    return block->tag;
}

//set the tag of the num-th(start with 0) block of the set-th set(start with 0) from the cache to the value of tag
void set_tag(struct Cache* cache, uint32_t set_index, uint32_t block_num, uint32_t tag) {
    uint64_t count = (uint64_t) ((set_index * cache->num_of_blocks + block_num) * cache->block_size); //calculate where this block is
    struct blockHeader* block = (struct blockHeader*) (cache->data + count);
    block->tag = tag;
}

//get the validity(0 or 1) of the num-th(start with 0) block of the set-th set(start with 0) from the cache, return is the validity
uint8_t get_valid(struct Cache* cache, uint32_t set_index, uint32_t block_num){
    uint64_t count = (uint64_t)((set_index * cache->num_of_blocks + block_num) * cache->block_size); //calculate where this block is
    struct blockHeader* block = (struct blockHeader*) (cache->data + count);
    return block->valid_bits;
}

//set the validity of the num-th(start with 0) block of the set-th set(start with 0) from the cache to the value of valid
void set_valid(struct Cache* cache, uint32_t set_index, uint32_t block_num, uint8_t valid){
    uint64_t count = (uint64_t)((set_index * cache->num_of_blocks + block_num) * cache->block_size); //calculate where this block is
    struct blockHeader* block = (struct blockHeader*) (cache->data + count);
    block->valid_bits = valid;
}

//get the dirty bit(0 or 1) of the num-th(start with 0) block of the set-th set(start with 0) from the cache, return is the dirty bit
uint8_t get_dirty(struct Cache* cache, uint32_t set_index, uint32_t block_num){
    uint64_t count = (uint64_t)((set_index * cache->num_of_blocks + block_num) * cache->block_size); //calculate where this block is
    struct blockHeader* block = (struct blockHeader*) (cache->data + count);
    return block->dirty_bits;
}

//set the dirty bit of the num-th(start with 0) block of the set-th set(start with 0) from the cache to the value of dirty
void set_dirty(struct Cache* cache, uint32_t set_index, uint32_t block_num, uint8_t dirty){
    uint64_t count = (uint64_t)((set_index * cache->num_of_blocks + block_num) * cache->block_size); //calculate where this block is
    struct blockHeader* block = (struct blockHeader*) (cache->data + count);
    block->dirty_bits = dirty;
}

//put a tag into the cache in the set's position, type identifies if it is a load or store(prior will "clean" the
//dirty bit while the latter will make a bit dirty)
void put_in(struct Cache* cache, uint32_t set_index, uint32_t block_num, uint32_t tag,char type) {
    if(cache->hit_strategy == 'b') {//special case for write back(so we are sure that it's write allocate)
        if(get_dirty(cache,set_index,block_num) == 1) {
            cache->cycles += (cache->num_of_bytes)/4 * 100; //we essentially put the existing one into the memory
        }
        if(type == 'l') {
            set_dirty(cache,set_index,block_num,0); //load makes the cache same as the memory, so it is no longer dirty
        } else {
            set_dirty(cache,set_index,block_num,1); //store makes the cache different from the memory(in case of write back), so change the dirty bit to 1
        }
    }

    cache->cycles += (cache->num_of_bytes)/4 * 100; //save the current data to the memory(since only misses can trigger put in)

    set_valid(cache,set_index,block_num,1); //make the block valid
    set_tag(cache,set_index,block_num,tag); //set its tag
}

//find a victim block from list, should be pointed by head, then adjust head pointer
uint32_t find_victim(struct Cache* cache, uint32_t set_index) {
    list_t* victim = *(cache->set_head_list + set_index);//firstly find the set(linked list) that the victim is at(it is also the pointer to the victim node)
    *(cache->set_head_list + set_index) = victim->next;//move the set's head to the next one(this one is going to be altered so it should be in the tail)
    return victim->block->block_num;//return the victim's position in the set so further operations(like put_in) can be done
}


