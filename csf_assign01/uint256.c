#include "uint256.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a UInt256 value from a single uint64_t value.
// Only the least-significant 64 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u64(uint64_t val) {
  // initiate a UInt256 struct
  UInt256 result;
  // the 0-th index (the least significant) is assigned with the passed-in
  // parameter
  result.data[0] = val;
  // the remaining three indexes are assigned with 0
  result.data[1] = 0;
  result.data[2] = 0;
  result.data[3] = 0;
  return result;
}

// Create a UInt256 value from an array of 4 uint64_t values.
// The element at index 0 is the least significant, and the element
// at index 3 is the most significant.
UInt256 uint256_create(const uint64_t data[4]) {
  // initiate a UInt256 struct
  UInt256 result;
  // assign each index with the respective uint64_t value in the array
  // parameter, with the struct index number corresponding to the array index
  result.data[0] = data[0];
  result.data[1] = data[1];
  result.data[2] = data[2];
  result.data[3] = data[3];
  return result;
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  // initiate a UInt256 struct
  UInt256 result;
  // create a char array for hex character input
  char segment[16] = {0};
  // iterate over the four indices in the UInt256 struct, and initiate all
  // uint64_t to 0
  for (int i = 0; i < 4; i++) {
    result.data[i] = 0;
  }
  // determine the length of the hex c-string to determine the upper bound of
  // the hex numerical representation
  int len = strlen(hex);
  int index = len / 16;
  int mod = len % 16;
  // iterate over the hex c-string. For each 16 digits (constituting one int64_t
  // value), fill the respective characters inside the initiated char array
  // segment
  for (int i = 0; i < index && i < 4; i++) {
    int in = 0;
    for (int m = len - 16 * (i + 1); m < len - 16 * i; ++m) {
      segment[in++] = hex[m];
    }
    // convert each char array to uint64_t type using build-in function strtoul
    result.data[i] = strtoul(segment, NULL, 16);
  }
  // if index is larger or equal to 4, it means the hex c-string is able to fill
  // UInt256's all four indices. Therefore the UInt256 struct can be returned
  if (index >= 4) {
    return result;
  }
  // elsewise, fill the remaining digits of the hex c-string again into the
  // segment char array and convert it to the last non-zero index in UInt256
  // struct, then return the struct
  else {
    int m = 0;
    for (; m < 16 - mod; ++m) {
      segment[m] = '0';
    }
    for (; m < 16; ++m) {
      segment[m] = hex[m + mod - 16];
    }
    result.data[index] = strtoul(segment, NULL, 16);
    return result;
  }
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  // initate the c-string hex as an empty string with only a null terminator
  int size = 1;
  char *hex = malloc(size * sizeof(char));
  hex[0] = '\0';
  // initiate a size 2 char array to store the current digit, and a boolean
  // has_1 to determine when the first non-zero digit occurs in the passed-in
  // parameter
  char new[2];   // store the current digit
  int i = 3;     // the current index value for UInt256
  int has_1 = 0; // check if there is a one before

  // iterate the indices of val.data
  for (; i > -1; i--) {
    uint64_t current = val.data[i]; // the current uint64 value
    // iterate over 16 times to get the 16 hex characters in one uint64_t value
    for (int j = 0; j < 16; j++) {
      // for each iteration, moves the uint64_t leftward to only use the top
      // four binary digit
      int result = current >> 60;
      // fill char array new with the corresponding hex character
      new[0] = (result < 10) ? (result + '0') : (result - 10 + 'a');
      new[1] = '\0';
      // check if a non-zero value appears
      // if so, it means that the numerical portion of val.data starts and the
      // c-string hex is reallocated for each iteration to encompass one more
      // digit
      if (new[0] != '0' && !has_1) {
        has_1 = 1;
      }
      if (has_1) {
        size++;
        hex = realloc(hex, size * sizeof(char));
        strcat(hex, new);
      }
      // at the end of each iteration, move the current uint64_t leftward by 4
      // binary digits so as to discard the 4 binary digits checked in this
      // iteration
      current = current << 4;
    }
  }
  // if all four uint64_t values are 0, then reallocate the hex c-string and add
  // a 0 before the null terminator
  if (!has_1) {
    hex = realloc(hex, ++size * sizeof(char));
    hex[0] = '0';
  }
  // ensure that the last digit of hex is a null terminator as the standard for
  // c-string
  hex[size - 1] = '\0';
  return hex;
}

// Get 64 bits of data from a UInt256 value.
// Index 0 is the least significant 64 bits, index 3 is the most
// significant 64 bits.
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  // initiate a uint64_t data variable
  uint64_t bits;
  // assign it with the value in the corresponding index of the struct
  bits = val.data[index];
  return bits;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
  // initate a UInt256 struct to store the sum and
  // initate the uint64_t at all indices to 0
  UInt256 sum;
  for (int i = 0; i < 4; i++) {
    sum.data[i] = 0;
  }
  uint64_t carry = 0;
  uint64_t left_digit = 0;
  uint64_t right_digit = 0;
  // iterate over the four indices and the 64 binary digits within each uint64_t
  // integer.
  for (int i = 0; i < 4; i++) {
    uint64_t current_left = left.data[i];
    uint64_t current_right = right.data[i];
    for (int j = 0; j < 64; j++) {
      // For each digit, obtain the binary digit at the left and right struct
      // and add the last carry.
      // then assign carry with the new value based on the result of the
      // addition
      left_digit = current_left % 2;
      right_digit = current_right % 2;
      // within each iteration, move the current left and right uint64_t
      // value rightward by 1 binary digit to discard the digit added in this
      // iteration.
      current_left = current_left >> 1;
      current_right = current_right >> 1;
      if (left_digit + right_digit + carry < 2) {
        sum.data[i] += ((left_digit + right_digit + carry) << j);
        carry = 0;
      } else {
        sum.data[i] += ((left_digit + right_digit + carry - 2) << j);
        carry = 1;
      }
    }
  }
  return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  // initate three structs with values of 0:
  // result to store the final substitution result
  // one which stores a numerical value of 1
  // right_complement which stores the complement struct for the passed_in
  // struct right
  UInt256 result;
  UInt256 one;
  UInt256 right_complement;
  for (int i = 0; i < 4; i++) {
    result.data[i] = 0;
    right_complement.data[i] = 0;
    one.data[i] = 0;
  }
  one.data[0]++;
  // create the complement struct for the right UInt 256
  for (int i = 0; i < 4; i++) {
    uint64_t current = right.data[i];
    for (int j = 0; j < 64; j++) {
      right_complement.data[i] += (1 - ((current >> j) % 2)) << j;
    }
  }
  // use the previously implemented uint256_add function to first add
  // right_complement by 1, then add the right_complement and the left struct to
  // obtain the subtration result
  right_complement = uint256_add(right_complement, one);
  result = uint256_add(right_complement, left);
  return result;
}

// a helper function for multiplication operation between UInt256 structs
// used for shifting a UInt256 struct by a designated binary digit number
UInt256 shift_by_index(UInt256 original, int digit) {
  // initate a struct to store the shifted original and initiate all values to 0
  UInt256 shifted;
  for (int i = 0; i < 4; i++) {
    shifted.data[i] = 0;
  }
  // determine the number of indices and remaining digit to shift
  int index = digit / 64;
  int carry = digit % 64;
  uint64_t carry_content[4] = {0, 0, 0, 0};
  // first shift entire uint64_int for the number of indices,
  for (int i = 0; i < 4 - index; i++) {
    shifted.data[i + index] = original.data[i];
  }
  if (carry == 0) {
    return shifted;
  }
  // determine the carry for the shifting of digits
  for (int i = 0; i < 3; i++) {
    carry_content[i + 1] = (shifted.data[i] >> (64 - carry));
  }
  // shift each uint64_t in the struct by the designated digit, then increment
  // the end with the carry content from the last index
  for (int i = 0; i < 4; i++) {
    shifted.data[i] = (shifted.data[i] << carry) + carry_content[i];
  }
  return shifted;
}
// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  // initate a struct to store the product values and initate all indices to 0
  UInt256 product;
  for (int i = 0; i < 4; i++) {
    product.data[i] = 0;
  }
  // break down the left value as a series of binary digit, 1 suggesting the
  // need to increment the product by the right struct shifted leftward by the
  // current left digit number
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 64; j++) {
      if ((left.data[i] >> j) % 2) {
        // the imcrementation process again applies the
        // previously implemented uint256_add function
        product = uint256_add(product, shift_by_index(right, i * 64 + j));
      }
    }
  }
  return product;
}
