#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uint256.h"

// Create a UInt256 value from a single uint64_t value.
// Only the least-significant 64 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u64(uint64_t val) {
  UInt256 result;
  result.data[0] = val;
  result.data[1] = 0;
  result.data[2] = 0;
  result.data[3] = 0;
  return result;
}

// Create a UInt256 value from an array of 4 uint64_t values.
// The element at index 0 is the least significant, and the element
// at index 3 is the most significant.
UInt256 uint256_create(const uint64_t data[4]) {
  UInt256 result;
  result.data[0] = data[0];
  result.data[1] = data[1];
  result.data[2] = data[2];
  result.data[3] = data[3];
  return result;
}
uint64_t convert_hex(const char hex, int exp) {
  uint64_t result = 0;
  if (hex <= '9') {
    result =  (hex - '0') << (4 * exp);
  } else {
    result = (hex - 'a' + 10) << (4 * exp); 
  }
  return result;
}
// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result;
  char segment[16] = {0};
  for(int i = 0; i < 4; i++){
    result.data[i] = 0;
  }
  int len = strlen(hex);
  int index = len / 16;
  int mod = len % 16;
  for(int i = 0; i < index && i < 4; i++){
    int in = 0;
    for (int m = len - 16*(i+1); m < len - 16 * i; ++m) {
      segment[in++] = hex[m];
    }
    result.data[i] = strtoul(segment, NULL, 16);
  }
  if(index >= 4){
    return result;
  }
  else{
    int m = 0;
    for (; m < 16 - mod; ++m) {
      segment[m] = '0';
    }
    for (; m < 16; ++m) {
      segment[m] = hex[m + mod - 16];
    }
    result.data[index] = strtoul(segment,NULL, 16);
    return result;
  }
  
  

  /*
  
  uint64_t result_64 = 0;
 
  int exp = 0; // current digit in uint64
  int i = 0; //current position in uint256
  for(int index = 0; index < len; index++){
    if(i == 4){
      return result;
    }
    result.data[i] += convert_hex(hex[len-1-index], exp++);
    printf("%lu\n", result.data[i]);
    if(exp == 16){
      exp = 0;
      i++;
      result_64 = 0;
    }
  }
  return result;
  */
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  int size = 1;
  char *hex = malloc(size * sizeof(char));
  hex[0] = '\0';

  char new[2]; // store the current digit
  int i = 3;
  int has_1 = 0; // check if there is a one before
  for(; i > -1; i--){
    uint64_t current = val.data[i]; // the current uint64 value
    for(int j = 0; j < 16; j++){
      int result = current >> 60;
      new[0] = (result < 10) ? (result + '0') : (result - 10 + 'a');
      new[1] = '\0';
      if(new[0] != '0' && !has_1){
        has_1 = 1;
      }
      if(has_1){
        size++;
        hex = realloc(hex, size * sizeof(char));
        strcat(hex, new);
      }
      current = current<< 4;
    }
  }
  if(!has_1){
    hex = realloc(hex, ++size * sizeof(char));
    hex[0] = '0';
  }
  hex[size - 1] = '\0';
  return hex;
}

// Get 64 bits of data from a UInt256 value.
// Index 0 is the least significant 64 bits, index 3 is the most
// significant 64 bits.
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  uint64_t bits;
  bits = val.data[index];
  return bits;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 sum;
  for(int i = 0; i < 4; i++){
    sum.data[i] = 0;
  }
  uint64_t carry = 0;
  uint64_t left_digit = 0;
  uint64_t right_digit = 0;
  for(int i = 0; i < 4; i++){
    uint64_t current_left = left.data[i];
    uint64_t current_right = right.data[i];
    for(int j = 0; j < 64; j++){
        left_digit = current_left % 2;
        right_digit = current_right % 2;
        current_left = current_left>>1;
        current_right = current_right>>1;
        if(left_digit + right_digit + carry < 2){
          sum.data[i] += ((left_digit + right_digit + carry) << j);
          carry = 0;
        }
        else{
          sum.data[i] += ((left_digit + right_digit + carry - 2) << j);
          carry = 1;
        }
    }
  }
  return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 result;
  UInt256 one;
  UInt256 right_complement;
  for(int i = 0; i < 4; i++){
    result.data[i] = 0;
    right_complement.data[i] = 0;
    one.data[i] = 0;
  }
  one.data[0]++;
  
  for(int i = 0; i < 4; i++){
    uint64_t current = right.data[i];
    for(int j = 0; j < 64; j++){
      right_complement.data[i] += (1 - ((current >> j) % 2)) << j; 
    }
  }
  right_complement = uint256_add(right_complement,one);
  result = uint256_add(right_complement , left);
  return result;
}

UInt256 shift_by_index(UInt256 original, int digit) {
    UInt256 shifted;
    for(int i = 0; i < 4; i++){
      shifted.data[i] = 0;
    }
    int index = digit / 64;
    int carry = digit % 64;
    uint64_t carry_content[4] = {0,0,0,0};
    for(int i = 0; i < 4 - index; i ++){
      shifted.data[i + index] = original.data[i];
    }
    if(carry == 0){
      return shifted;
    }
    for(int i = 0; i < 3; i++){
      carry_content[i+1] = (shifted.data[i]>>(64-carry));
    }
    for(int i = 0; i < 4; i++){
      shifted.data[i] = (shifted.data[i]<<carry) + carry_content[i];
    }
    return shifted;


}
// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product;
  for(int i = 0; i < 4; i++){
    product.data[i] = 0;
  }
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 64; j++){
      if((left.data[i]>>j ) % 2){
        product = uint256_add(product, shift_by_index(right, i * 64 + j));
      }
      
    }
  }
  return product;
}
