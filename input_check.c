/*
 * functions that check the validity of the command line inputs of csim(cache simulation)
 * CSF Assignment 3
 * Richard Zhang
 * rzhang89@jhu.edu
 */

#include "input_check.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//check if the inputs are valid
//argc is the number of inputs, argv is the array that stores the inputs
//return 0 for invalid, 1 for valid
uint8_t check_inputs (int argc, char* argv[]) {
    if (argc != 7) {
        fprintf(stderr,"error: incorrect number of arguments\n");
        return 0; //incorrect number of inputs
    }
    
    if (!number_checks(argv)) { //if we have invalid integer inputs(either not power of two or simply not an integer), return false
        return 0;
    }

    if (strcmp("write-allocate",argv[4]) && strcmp("no-write-allocate",argv[4])) {
        fprintf(stderr,"error: must be write-allocate or no-write allocate\n");
        return 0; //if it is neither write-allocate nor no-write-allocate, we have an unknown type so return 1
    }

    if (strcmp("write-through",argv[5]) && strcmp("write-back",argv[5])) {
        fprintf(stderr,"error: must be either write-through or write-back\n");
        return 0; //if it is neither write-back nor write-through, we have an unknown type so return 1
    }

    if (strcmp("lru",argv[6]) && strcmp("fifo",argv[6])) {
        fprintf(stderr,"error: must be either lru or fifo\n");
        return 0; //if it is neither lru nor fifo, we have an unknown type so return 1
    }

    if (!strcmp("no-write-allocate",argv[4]) && !strcmp("write-back",argv[5])) {
        fprintf(stderr,"error: no-write-allocate and write-back must not coexist\n");
        return 0; //can't have no write allocate and write back in the same time
    }

    return 1;
}

//checks if the input integers(# of set in cache, # of blocks in set, # of bytes in block) are valid
//argv is the array that contains the inputs
//return 1 for valid, 0 for invalid
uint8_t number_checks(char* argv[]) {
    if(!is_non_negative_integer(argv[1], strlen(argv[1])) || !is_non_negative_integer(argv[2], strlen(argv[2])) || !is_non_negative_integer(argv[3], strlen(argv[3]))) {
        fprintf(stderr,"error: number of sets in the cache, number of blocks in each set, and number of bytes in each block must all be positive integers\n");
        return 0;
    }
    if (atoi(argv[3]) < 4 || is_power_of_two(atoi(argv[3])) == 0) {
        fprintf(stderr,"error: invalid block size\n");
        return 0; //if the block size < 4 or is not a power of two, invalid blocksize
    }
    if (!is_power_of_two(atoi(argv[1]))) {
        fprintf(stderr,"error: invalid number of sets(not a power of two)\n");
        return 0; //number of sets is not a power of 2
    }

    if (!is_power_of_two(atoi(argv[2]))) {
        fprintf(stderr,"error: invalid number of blocks in each set(not a power of two)\n");
        return 0; //number of blocks in each set is not a power of 2
    }
    return 1;
}

//checks if a number(the parameter number) is a power of 2. Return 1 for yes, 0 for no
uint8_t is_power_of_two (uint32_t number){
    if (number == 0) {
        return 0; //zero is a special case and it's not a power of two
    }
    number &= (number-1);
    //basically for a number to be a power of two, itself & itself-1 must be 0
    if (number == 0) {
        return 1;
    }
    return 0;
}

//checks if a string represents a non-negative integer
//str is the pointer to the start of this string, length means the length of the string(excluding the null terminator)
//return 0 if false, 1 if true
uint8_t is_non_negative_integer(char* str,uint64_t length) {
    for (uint64_t i = 0;i < length; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return 0; //if we have non digit numbers, that means the input is not a positive integer
        }
    }
    return 1;
}