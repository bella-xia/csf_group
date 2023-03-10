/*
 * functions that check the validity of the command line inputs of csim(cache simulation)
 * CSF Assignment 3
 * Richard Zhang
 * rzhang89@jhu.edu
 */

#ifndef INPUT_CHECK_H
#define INPUT_CHECK_H

#include <stdint.h>

//check if the inputs are valid
//argc is the number of inputs, argv is the array that stores the inputs
//return 0 for invalid, 1 for valid
uint8_t check_inputs (int argc, char* argv[]);

//checks if the input integers(# of set in cache, # of blocks in set, # of bytes in block) are valid
//argv is the array that contains the inputs
//return 1 for valid, 0 for invalid
uint8_t number_checks(char* argv[]);

//checks if a number(the parameter number) is a power of 2. Return 1 for yes, 0 for no
uint8_t is_power_of_two (uint32_t number);

//checks if a string represents a non-negative integer
//str is the pointer to the start of this string, length means the length of the string(excluding the null terminator)
//return 0 if false, 1 if true
uint8_t is_non_negative_integer(char* str,uint64_t length);

#endif