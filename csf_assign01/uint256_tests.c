#include "tctest.h"
#include "uint256.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  UInt256 zero;
  UInt256 one;
  UInt256 large1;
  UInt256 large2;
  UInt256 large3;
  UInt256 large4;
  UInt256 large5;
  UInt256 large6;
  UInt256 large7;
  UInt256 largest;

  const char *hex1;
  const char *hex2;
  const char *hex3;
  const char *hex_large_2;
  const char *hex_large_3;
  const char *hex_large_4;
  const char *hex_large_5;
  const char *hex_large_6;
  const char *hex_large_7;
  const char *hex_largest;

} TestObjs;

// Functions to create and cleanup the test fixture object
TestObjs *setup(void);
void cleanup(TestObjs *objs);

// Helper functions for implementing tests
int check(UInt256 val, uint64_t val3, uint64_t val2, uint64_t val1,
          uint64_t val0);
int check_Uint256(UInt256 val1,UInt256 val2);
// Declarations of test functions
void test_get_bits(TestObjs *objs);
void test_create_from_u64(TestObjs *objs);
void test_create(TestObjs *objs);
void test_create_from_hex(TestObjs *objs);
void test_format_as_hex(TestObjs *objs);
void test_add_1(TestObjs *objs);
void test_add_2(TestObjs *objs);
void test_add_3(TestObjs *objs);
void test_sub_1(TestObjs *objs);
void test_sub_2(TestObjs *objs);
void test_sub_3(TestObjs *objs);
void test_mul_1(TestObjs *objs);
void test_mul_2(TestObjs *objs);
void test_add_rand_01();
void test_add_rand_02();
void test_max_add_one();
void test_num_add_zero();
void test_add_overflow_01();
void test_add_overflow_02();
void test_sub_rand_01();
void test_sub_rand_02();
void test_num_minus_zero();
void test_zero_minus_one();
void test_sub_overflow_01();
void test_sub_overflow_02();
void test_sub_overflow_03();
void test_mul_rand_01();
void test_mul_rand_02();
void test_mul_zero();
void test_mul_one();
void test_mul_overload_01();
void test_mul_overload_02();

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();

  TEST(test_get_bits);
  TEST(test_create_from_u64);
  TEST(test_create);
  TEST(test_create_from_hex);
  TEST(test_format_as_hex);
  TEST(test_add_1);
  TEST(test_add_2);
  TEST(test_add_3);
  TEST(test_sub_1);
  TEST(test_sub_2);
  TEST(test_sub_3);
  TEST(test_mul_1);
  TEST(test_mul_2);
  TEST(test_add_rand_01);
  TEST(test_add_rand_02);
  TEST(test_max_add_one);
  TEST(test_num_add_zero);
  TEST(test_add_overflow_01);
  TEST(test_add_overflow_02);
  TEST(test_sub_rand_01);
  TEST(test_sub_rand_02);
  TEST(test_num_minus_zero);
  TEST(test_zero_minus_one);
  TEST(test_sub_overflow_01);
  TEST(test_sub_overflow_02);
  TEST(test_sub_overflow_03);
  TEST(test_mul_rand_01);
  TEST(test_mul_rand_02);
  TEST(test_mul_zero);
  TEST(test_mul_one);
  TEST(test_mul_overload_01);
  TEST(test_mul_overload_02);
  TEST_FINI();
}

TestObjs *setup(void) {
  TestObjs *objs = (TestObjs *)malloc(sizeof(TestObjs));

  // initialize several UInt256 values "manually"

  objs->zero.data[3] = 0U;
  objs->zero.data[2] = 0U;
  objs->zero.data[1] = 0U;
  objs->zero.data[0] = 0U;

  objs->one.data[3] = 0U;
  objs->one.data[2] = 0U;
  objs->one.data[1] = 0U;
  objs->one.data[0] = 1U;

  objs->large1.data[3] = 0xAA;
  objs->large1.data[2] = 0xBB;
  objs->large1.data[1] = 0xCC;
  objs->large1.data[0] = 0xDD;

  objs->large2.data[0] = 0x78dc37b7f6c25a78;
  objs->large2.data[1] = 0x3a471b8899988a4e;
  objs->large2.data[2] = 0x90765a1d347a169d;
  objs->large2.data[3] = 0x26eaeaef193116d;

  objs->large3.data[0] = 0x9bc3183d4b8b34a8;
  objs->large3.data[1] = 0xf6f3fdecc5f0191e;
  objs->large3.data[2] = 0xa9aa8cc928a43ac0;
  objs->large3.data[3] = 0x96fc36d9e545a34;

  objs->large4.data[0] = 0x149f4ff5424d8f20;
  objs->large4.data[1] = 0x313b19755f88a36d;
  objs->large4.data[2] = 0x3a20e6e65d1e515e;
  objs->large4.data[3] = 0xbde721c8fe76ba2;

  objs->large5.data[0] = 0x935787357eea6e4c;
  objs->large5.data[1] = 0x0308df893257e525;
  objs->large5.data[2] = 0x07c3c720a42b2bd0;
  objs->large5.data[3] = 0x7c5d7fc2b102f37;

  objs->large6.data[0] = 0x88a4371602842fc4;
  objs->large6.data[1] = 0x01900518911370d3;
  objs->large6.data[2] = 0x47e4be9e64ce17b0;
  objs->large6.data[3] = 0x5c963dff57abb12;

  objs->large7.data[0] = 0x0ab3501f7c663e88;
  objs->large7.data[1] = 0x0178da70a1447452;
  objs->large7.data[2] = 0xbfdf08823f5d1420;
  objs->large7.data[3] = 0x1fc741c35957424;
  //largest value possible
  objs->largest.data[0] = 0xffffffffffffffff;
  objs->largest.data[1] = 0xffffffffffffffff;
  objs->largest.data[2] = 0xffffffffffffffff;
  objs->largest.data[3] = 0xffffffffffffffff;

  

  // example hex strings

  objs->hex1 = "0";
  objs->hex2 = "cafe";
  objs->hex3 = "4a4b72ebb654226ef77ed83d884f4940e4243bc3913ceaf5781b28d25fb00b0";
  objs->hex_large_2 = "26eaeaef193116d90765a1d347a169d3a471b8899988a4e78dc37b7f6c25a78";
  objs->hex_large_3 = "96fc36d9e545a34a9aa8cc928a43ac0f6f3fdecc5f0191e9bc3183d4b8b34a8";
  objs->hex_large_4 = "bde721c8fe76ba23a20e6e65d1e515e313b19755f88a36d149f4ff5424d8f20";
  objs->hex_large_5 = "7c5d7fc2b102f3707c3c720a42b2bd00308df893257e525935787357eea6e4c";
  objs->hex_large_6 = "5c963dff57abb1247e4be9e64ce17b001900518911370d388a4371602842fc4";
  objs->hex_large_7 = "1fc741c35957424bfdf08823f5d14200178da70a14474520ab3501f7c663e88";
  //largest value possible
  objs->hex_largest = "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";

  return objs;
}

void cleanup(TestObjs *objs) { free(objs); }

int check(UInt256 val, uint64_t val3, uint64_t val2, uint64_t val1,
          uint64_t val0) {
  return uint256_get_bits(val, 3) == val3 && uint256_get_bits(val, 2) == val2 &&
         uint256_get_bits(val, 1) == val1 && uint256_get_bits(val, 0) == val0;
}
int check_Uint256(UInt256 val1, UInt256 val2){
  return uint256_get_bits(val1, 3) == uint256_get_bits(val2, 3) && uint256_get_bits(val1, 2) == uint256_get_bits(val2, 2) &&
         uint256_get_bits(val1, 1) == uint256_get_bits(val2, 1) && uint256_get_bits(val1, 0) == uint256_get_bits(val2, 0);
}

void test_get_bits(TestObjs *objs) {
  ASSERT(0U == uint256_get_bits(objs->zero, 3));
  ASSERT(0U == uint256_get_bits(objs->zero, 2));
  ASSERT(0U == uint256_get_bits(objs->zero, 1));
  ASSERT(0U == uint256_get_bits(objs->zero, 0));

  ASSERT(0U == uint256_get_bits(objs->one, 3));
  ASSERT(0U == uint256_get_bits(objs->one, 2));
  ASSERT(0U == uint256_get_bits(objs->one, 1));
  ASSERT(1U == uint256_get_bits(objs->one, 0));
}

void test_create_from_u64(TestObjs *objs) {
  objs->zero = uint256_create_from_u64(0U);
  objs->one = uint256_create_from_u64(1U);

  ASSERT(check(objs->zero, 0U, 0U, 0U, 0U));
  ASSERT(check(objs->one, 0U, 0U, 0U, 1U));
}

void test_create(TestObjs *objs) {
  // Note that the initializers list the uint64_t values to
  // be stored in order from least significant to most significant
  uint64_t zero_init[] = {0U, 0U, 0U, 0U};
  uint64_t one_init[] = {1U, 0U, 0U, 0U};

  objs->zero = uint256_create(zero_init);
  objs->one = uint256_create(one_init);

  // In the calls to check, the uint64_t values are specified
  // in the order most significant to least significant
  // (i.e., the reverse of the order in the array passed to
  // uint256_create)
  ASSERT(check(objs->zero, 0U, 0U, 0U, 0U));
  ASSERT(check(objs->one, 0U, 0U, 0U, 1U));
}

void test_create_from_hex(TestObjs *objs) {
  UInt256 val;

  val = uint256_create_from_hex(objs->hex1);
  ASSERT(check(val, 0x0UL, 0x0UL, 0x0UL, 0x0UL));

  val = uint256_create_from_hex(objs->hex2);
  ASSERT(check(val, 0x0UL, 0x0UL, 0x0UL, 0xcafeUL));

  val = uint256_create_from_hex(objs->hex_large_2);
  ASSERT(check_Uint256(val,objs->large2));

  val = uint256_create_from_hex(objs->hex_large_3);
  ASSERT(check_Uint256(val,objs->large3));

  val = uint256_create_from_hex(objs->hex_large_4);
  ASSERT(check_Uint256(val,objs->large4));
  
  val = uint256_create_from_hex(objs->hex_large_5);
  ASSERT(check_Uint256(val,objs->large5));

  val = uint256_create_from_hex(objs->hex_large_6);
  ASSERT(check_Uint256(val,objs->large6));

  val = uint256_create_from_hex(objs->hex_large_7);
  ASSERT(check_Uint256(val,objs->large7));

  val = uint256_create_from_hex(objs->hex_largest);
  ASSERT(check_Uint256(val,objs->largest));
  
}

void test_format_as_hex(TestObjs *objs) {
  char *s;

  s = uint256_format_as_hex(objs->zero);
  ASSERT(0 == strcmp("0", s));
  free(s);

  s = uint256_format_as_hex(objs->one);
  ASSERT(0 == strcmp("1", s));
  free(s);

  s = uint256_format_as_hex(objs->large2);
  ASSERT(0 == strcmp(objs->hex_large_2, s));
  free(s);
  
  s = uint256_format_as_hex(objs->large3);
  ASSERT(0 == strcmp(objs->hex_large_3, s));
  free(s);
  
  s = uint256_format_as_hex(objs->large4);
  ASSERT(0 == strcmp(objs->hex_large_4, s));
  free(s);
  
  s = uint256_format_as_hex(objs->large5);
  ASSERT(0 == strcmp(objs->hex_large_5, s));
  free(s);
  
  s = uint256_format_as_hex(objs->large6);
  ASSERT(0 == strcmp(objs->hex_large_6, s));
  free(s);
  
  s = uint256_format_as_hex(objs->large7);
  ASSERT(0 == strcmp(objs->hex_large_7, s));
  free(s);
  
  s = uint256_format_as_hex(objs->largest);
  ASSERT(0 == strcmp(objs->hex_largest, s));
  free(s);
  
}

void test_add_1(TestObjs *objs) {
  // basic addition tests

  UInt256 sum;

  sum = uint256_add(objs->zero, objs->one);

  ASSERT(0UL == sum.data[3]);
  ASSERT(0UL == sum.data[2]);
  ASSERT(0UL == sum.data[1]);
  ASSERT(1UL == sum.data[0]);
}

void test_add_2(TestObjs *objs) {
  // "medium" addition tests

  (void)objs;

  UInt256 left, right, result;

  // a932467b27ccb2b4cb6c96c4810cb5e + b37aef92a5179d60af2f9cad0a4b38a =
  // 15cad360dcce450157a9c33718b57ee8
  left.data[0] = 0x4cb6c96c4810cb5eUL;
  left.data[1] = 0xa932467b27ccb2bUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x0af2f9cad0a4b38aUL;
  right.data[1] = 0xb37aef92a5179d6UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_add(left, right);
  ASSERT(0x57a9c33718b57ee8UL == result.data[0]);
  ASSERT(0x15cad360dcce4501UL == result.data[1]);
  ASSERT(0x0UL == result.data[2]);
  ASSERT(0x0UL == result.data[3]);
}

void test_add_3(TestObjs *objs) {
  // "large" addition tests

  (void)objs;

  UInt256 left, right, result;

  // 9515af0631ecc4779e0c122009e87b1ff1076115dab87f8190bc8c2ceb84ceb +
  // 173ba2210b102e74dc94683c12d20ceda74af7d4cf5fc80aa154504e7e58712 =
  // ac5151273cfcf2ec7aa07a5c1cba880d985258eaaa18478c3210dc7b69dd3fd
  left.data[0] = 0x190bc8c2ceb84cebUL;
  left.data[1] = 0xff1076115dab87f8UL;
  left.data[2] = 0x79e0c122009e87b1UL;
  left.data[3] = 0x9515af0631ecc47UL;
  right.data[0] = 0xaa154504e7e58712UL;
  right.data[1] = 0xda74af7d4cf5fc80UL;
  right.data[2] = 0x4dc94683c12d20ceUL;
  right.data[3] = 0x173ba2210b102e7UL;
  result = uint256_add(left, right);
  ASSERT(0xc3210dc7b69dd3fdUL == result.data[0]);
  ASSERT(0xd985258eaaa18478UL == result.data[1]);
  ASSERT(0xc7aa07a5c1cba880UL == result.data[2]);
  ASSERT(0xac5151273cfcf2eUL == result.data[3]);
}

void test_sub_1(TestObjs *objs) {
  // basic subtraction tests

  UInt256 result;

  result = uint256_sub(objs->one, objs->zero);
  ASSERT(0UL == result.data[3]);
  ASSERT(0UL == result.data[2]);
  ASSERT(0UL == result.data[1]);
  ASSERT(1UL == result.data[0]);
}

void test_sub_2(TestObjs *objs) {
  // "medium" subtraction tests

  (void)objs;

  UInt256 left, right, result;

  // af7939833038cfa010f734368aaad66 - 6473865d77af47d71f7aedf64364960 =
  // 4b05b325b88987c8f17c46404746406
  left.data[0] = 0x010f734368aaad66UL;
  left.data[1] = 0xaf7939833038cfaUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x71f7aedf64364960UL;
  right.data[1] = 0x6473865d77af47dUL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_sub(left, right);
  ASSERT(0x8f17c46404746406UL == result.data[0]);
  ASSERT(0x4b05b325b88987cUL == result.data[1]);
  ASSERT(0x0UL == result.data[2]);
  ASSERT(0x0UL == result.data[3]);
}

void test_sub_3(TestObjs *objs) {
  // "large" subtraction tests

  (void)objs;

  UInt256 left, right, result;

  // bc556287a225313cc07a1509f4ebb335034f5d413945ac7d0bdb42962a6ae8c -
  // 7209ef9bebd10ecdc8fb3ccc6c9c69f41f2b217da808c18793c019c3cabaddc =
  // 4a4b72ebb654226ef77ed83d884f4940e4243bc3913ceaf5781b28d25fb00b0
  left.data[0] = 0xd0bdb42962a6ae8cUL;
  left.data[1] = 0x5034f5d413945ac7UL;
  left.data[2] = 0xcc07a1509f4ebb33UL;
  left.data[3] = 0xbc556287a225313UL;
  right.data[0] = 0x793c019c3cabaddcUL;
  right.data[1] = 0x41f2b217da808c18UL;
  right.data[2] = 0xdc8fb3ccc6c9c69fUL;
  right.data[3] = 0x7209ef9bebd10ecUL;
  result = uint256_sub(left, right);
  ASSERT(0x5781b28d25fb00b0UL == result.data[0]);
  ASSERT(0x0e4243bc3913ceafUL == result.data[1]);
  ASSERT(0xef77ed83d884f494UL == result.data[2]);
  ASSERT(0x4a4b72ebb654226UL == result.data[3]);
}

void test_mul_1(TestObjs *objs) {
  // basic multiplication tests

  UInt256 result;

  result = uint256_mul(objs->one, objs->one);
  ASSERT(check(result, 0UL, 0UL, 0UL, 1UL));
  result = uint256_mul(objs->one, objs->zero);
  ASSERT(check(result, 0UL, 0UL, 0UL, 0UL));
}

void test_mul_2(TestObjs *objs) {
  (void)objs;

  UInt256 left, right, result;

  // 761544a98b82abc63f23766d1391782 * 14bf658bd8053a9484c32d955a47a2f =
  // 991f2125eacd361abad710163aa9be6117fa57cddf52e73c97a28d7f744de
  left.data[0] = 0x63f23766d1391782UL;
  left.data[1] = 0x761544a98b82abcUL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;
  right.data[0] = 0x484c32d955a47a2fUL;
  right.data[1] = 0x14bf658bd8053a9UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;
  result = uint256_mul(left, right);
  ASSERT(0x73c97a28d7f744deUL == result.data[0]);
  ASSERT(0xe6117fa57cddf52eUL == result.data[1]);
  ASSERT(0x61abad710163aa9bUL == result.data[2]);
  ASSERT(0x991f2125eacd3UL == result.data[3]);
}

void test_add_rand_01() {
  // genfact.rb generated random test
  // aecfe8a9eaec157028b3f0708698fcb447853456855d62d82cb12abb0c6bc60 +
  // f8bd7febfbe6c62f500bc06efdf5d5ca75f67211cab9b7248c945784d090a6e =
  // 1a78d6895e6d2db9f78bfb0df848ed27ebd7ba668501719fcb945823fdcfc6ce
  UInt256 left, right, result;
  left.data[0] = 0x82cb12abb0c6bc60UL;
  left.data[1] = 0x447853456855d62dUL;
  left.data[2] = 0x028b3f0708698fcbUL;
  left.data[3] = 0xaecfe8a9eaec157UL;

  right.data[0] = 0x48c945784d090a6eUL;
  right.data[1] = 0xa75f67211cab9b72UL;
  right.data[2] = 0xf500bc06efdf5d5cUL;
  right.data[3] = 0xf8bd7febfbe6c62UL;

  result = uint256_add(left, right);

  ASSERT(0xcb945823fdcfc6ceUL == result.data[0]);
  ASSERT(0xebd7ba668501719fUL == result.data[1]);
  ASSERT(0xf78bfb0df848ed27UL == result.data[2]);
  ASSERT(0x1a78d6895e6d2db9UL == result.data[3]);
}

void test_add_rand_02() {
  // genfact.rb generated random test
  // 28a9e316e6045c8a1c40c722fbdc661959966155091d1feb7d269ea02fa7fcf +
  // d4ce4ad524809e41d6ab3506fce5f7d99eda2598ac8cbb4847a4a7b77e334b4 =
  // fd782dec0a84facbf2ebfc29f8c25df2f87086edb5a9db33c4cb4657addb483
  UInt256 left, right, result;
  left.data[0] = 0xb7d269ea02fa7fcfUL;
  left.data[1] = 0x959966155091d1feUL;
  left.data[2] = 0xa1c40c722fbdc661UL;
  left.data[3] = 0x28a9e316e6045c8UL;

  right.data[0] = 0x847a4a7b77e334b4UL;
  right.data[1] = 0x99eda2598ac8cbb4UL;
  right.data[2] = 0x1d6ab3506fce5f7dUL;
  right.data[3] = 0xd4ce4ad524809e4UL;

  result = uint256_add(left, right);

  ASSERT(0x3c4cb4657addb483UL == result.data[0]);
  ASSERT(0x2f87086edb5a9db3UL == result.data[1]);
  ASSERT(0xbf2ebfc29f8c25dfUL == result.data[2]);
  ASSERT(0xfd782dec0a84facUL == result.data[3]);
}

void test_max_add_one() {
  // test if the overflow result of the maximum value and 1 gives 0
  UInt256 max;
  for (int i = 0; i < 4; ++i) {
    max.data[i] = ~(0UL);
  }

  UInt256 one = uint256_create_from_u64(1UL);

  UInt256 sum = uint256_add(max, one);

  // these assertions should succeed
  ASSERT(sum.data[3] == 0UL);
  ASSERT(sum.data[2] == 0UL);
  ASSERT(sum.data[1] == 0UL);
  ASSERT(sum.data[0] == 0UL);
}

void test_num_add_zero() {
  // test the result of a number plus 0
  UInt256 zero = uint256_create_from_u64(0UL);
  UInt256 num;
  num.data[0] = 0x155e1561555ba8b0UL;
  num.data[1] = 0x13fa29a2df6621d3UL;
  num.data[2] = 0xc717cbf3fcdc8e72UL;
  num.data[3] = 0x5dd21180d360a8aUL;
  UInt256 result = uint256_add(zero, num);
  for (int i = 0; i < 4; ++i) {
    ASSERT(num.data[i] == result.data[i]);
  }
}

void test_add_overflow_01() {
  // test the addition of two numbers that can cause an overload
  // aae98d4eed8a0a69d9b922a68b34e756b293cba43aa407483f7d60bafa480828 +
  // 75d0c78f48ceff1bf352468225f9b1e468a6a0ff854f6e27fb284c3f33101d85 =
  // 10ba54de36590985cd0b6928b12e993b1b3a6ca3bff375703aa5acfa2d5825ad
  UInt256 left, right, result;
  left.data[0] = 0x3f7d60bafa480828UL;
  left.data[1] = 0xb293cba43aa40748UL;
  left.data[2] = 0xd9b922a68b34e756UL;
  left.data[3] = 0xaae98d4eed8a0a69UL;

  right.data[0] = 0xfb284c3f33101d85UL;
  right.data[1] = 0x68a6a0ff854f6e27UL;
  right.data[2] = 0xf352468225f9b1e4UL;
  right.data[3] = 0x75d0c78f48ceff1bUL;

  result = uint256_add(left, right);

  ASSERT(0x3aa5acfa2d5825adUL == result.data[0]);
  ASSERT(0x1b3a6ca3bff37570UL == result.data[1]);
  ASSERT(0xcd0b6928b12e993bUL == result.data[2]);
  ASSERT(0x20ba54de36590985UL == result.data[3]);
}

void test_add_overflow_02() {
  // test the addition of two numbers that can cause an overload
  // (max - 1) + ff7262482d12145eb2202f12aef5611de429528993ed7808ec1d270ececccf
  // = ff7262482d12145eb2202f12aef5611de429528993ed7808ec1d270ececccd
  UInt256 left, right, result;
  for (int i = 1; i < 4; ++i) {
    left.data[i] = ~(0UL);
  }
  left.data[0] = ~(0UL) - 1;

  right.data[0] = 0x08ec1d270ececccfUL;
  right.data[1] = 0x1de429528993ed78UL;
  right.data[2] = 0x5eb2202f12aef561UL;
  right.data[3] = 0xff7262482d1214UL;

  result = uint256_add(left, right);

  ASSERT(0x08ec1d270ececccdUL == result.data[0]);
  ASSERT(0x1de429528993ed78UL == result.data[1]);
  ASSERT(0x5eb2202f12aef561UL == result.data[2]);
  ASSERT(0xff7262482d1214UL == result.data[3]);
}

void test_sub_rand_01() {
  // genfact.rb generated random test
  // 5dd21180d360a8ac717cbf3fcdc8e7213fa29a2df6621d3155e1561555ba8b0 -
  // 554c01cb56553b8e01219c50506509ba92d647107cd5ea884c2a7a65b854d75 =
  // 8860fb57d0b6d1e705b22ef7d63dd66accc531d798c32a909b6dbaf9d65b3b
  UInt256 left, right, result;
  left.data[0] = 0x155e1561555ba8b0UL;
  left.data[1] = 0x13fa29a2df6621d3UL;
  left.data[2] = 0xc717cbf3fcdc8e72UL;
  left.data[3] = 0x5dd21180d360a8aUL;

  right.data[0] = 0x84c2a7a65b854d75UL;
  right.data[1] = 0xa92d647107cd5ea8UL;
  right.data[2] = 0xe01219c50506509bUL;
  right.data[3] = 0x554c01cb56553b8UL;

  result = uint256_sub(left, right);

  ASSERT(0x909b6dbaf9d65b3bUL == result.data[0]);
  ASSERT(0x6accc531d798c32aUL == result.data[1]);
  ASSERT(0xe705b22ef7d63dd6UL == result.data[2]);
  ASSERT(0x8860fb57d0b6d1UL == result.data[3]);
}

void test_sub_rand_02() {
  // genfact.rb generated random test
  // ead237b821c22eada60a9c502595e6684353108be980d622a29bfa10c6b4fb6 -
  // 43763d2d32248d2fc3588d6f1f3c1dfcdb140ecca492985e1e438bf984a4906 =
  // a75bfa8aef9da17de2b20ee10659c86b683f01bf44ee3dc484586e1742106b0
  UInt256 left, right, result;
  left.data[0] = 0x2a29bfa10c6b4fb6UL;
  left.data[1] = 0x84353108be980d62UL;
  left.data[2] = 0xda60a9c502595e66UL;
  left.data[3] = 0xead237b821c22eaUL;

  right.data[0] = 0xe1e438bf984a4906UL;
  right.data[1] = 0xcdb140ecca492985UL;
  right.data[2] = 0xfc3588d6f1f3c1dfUL;
  right.data[3] = 0x43763d2d32248d2UL;

  result = uint256_sub(left, right);

  ASSERT(0x484586e1742106b0UL == result.data[0]);
  ASSERT(0xb683f01bf44ee3dcUL == result.data[1]);
  ASSERT(0xde2b20ee10659c86UL == result.data[2]);
  ASSERT(0xa75bfa8aef9da17UL == result.data[3]);
}

void test_num_minus_zero() {
  // test the result of a number minus zero
  // ffa0f0de995bbb1d6073aeed4a3d2d2af59d0e64a07de8ca116e0baf551fd4 - 0 =
  // ffa0f0de995bbb1d6073aeed4a3d2d2af59d0e64a07de8ca116e0baf551fd4

  UInt256 zero = uint256_create_from_u64(0UL);
  UInt256 left, result;
  left.data[0] = 0xca116e0baf551fd4UL;
  left.data[1] = 0x2af59d0e64a07de8UL;
  left.data[2] = 0x1d6073aeed4a3d2dUL;
  left.data[3] = 0xffa0f0de995bbbUL;
  result = uint256_sub(left, zero);
  for (int i = 0; i < 4; ++i) {
    ASSERT(left.data[i] == result.data[i]);
  }
}

void test_zero_minus_one() {
  // test the case of 0 - 1, which should result in the maximum value
  UInt256 zero = uint256_create_from_u64(0UL);
  UInt256 one = uint256_create_from_u64(1UL);
  UInt256 result = uint256_sub(zero, one);
  for (int i = 0; i < 4; ++i) {
    ASSERT(~(0UL) == result.data[i]);
  }
}

void test_sub_overflow_01() {
  // 81cf2e624a6e4034e3e71dd397d0ee17820ac2436bf7ccc122e46969415ddf3 -
  // cbe26547255b3e3f66e444210c88f90b9c17e8d8dd0082dae7d10a7ee8aa019 =
  // fb5ecc91b251301f57d02d9b28b47f50be5f2d96a8ef749e63b135eea58b3dda
  UInt256 left, right, result;
  left.data[0] = 0x122e46969415ddf3UL;
  left.data[1] = 0x7820ac2436bf7cccUL;
  left.data[2] = 0x4e3e71dd397d0ee1UL;
  left.data[3] = 0x81cf2e624a6e403UL;

  right.data[0] = 0xae7d10a7ee8aa019UL;
  right.data[1] = 0xb9c17e8d8dd0082dUL;
  right.data[2] = 0xf66e444210c88f90UL;
  right.data[3] = 0xcbe26547255b3e3UL;

  result = uint256_sub(left, right);

  ASSERT(0x63b135eea58b3ddaUL == result.data[0]);
  ASSERT(0xbe5f2d96a8ef749eUL == result.data[1]);
  ASSERT(0x57d02d9b28b47f50UL == result.data[2]);
  ASSERT(0xfb5ecc91b251301fUL == result.data[3]);
}

void test_sub_overflow_02() {
  // 11f1b91e0300186ec28b30deae8fe02c7bd3ddb73798cdee490170b0d69ed4a -
  // dbfdfb2da1c628322768436b4243c5ec993b5bcd281551d3fdb5d870858884d =
  // 35f3bdf06139f03c9b22ed736c4c1a3fe29881ea0f837c1a4b4b984051164fd
  UInt256 left, right, result;
  left.data[0] = 0xe490170b0d69ed4aUL;
  left.data[1] = 0xc7bd3ddb73798cdeUL;
  left.data[2] = 0xec28b30deae8fe02UL;
  left.data[3] = 0x11f1b91e0300186UL;

  right.data[0] = 0x3fdb5d870858884dUL;
  right.data[1] = 0xc993b5bcd281551dUL;
  right.data[2] = 0x22768436b4243c5eUL;
  right.data[3] = 0xdbfdfb2da1c6283UL;

  result = uint256_sub(left, right);

  ASSERT(0xa4b4b984051164fdUL == result.data[0]);
  ASSERT(0xfe29881ea0f837c1UL == result.data[1]);
  ASSERT(0xc9b22ed736c4c1a3UL == result.data[2]);
  ASSERT(0xf35f3bdf06139f03UL == result.data[3]);
}

void test_sub_overflow_03() {
  // 5f8 - 170d =
  // ffffffffffffffff ffffffffffffffff ffffffffffffffff ffffffffffffeeeb
  UInt256 left, right, result;
  left.data[0] = 0x5f8UL;
  left.data[1] = 0x0UL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;

  right.data[0] = 0x170dUL;
  right.data[1] = 0x0UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;

  result = uint256_sub(left, right);

  ASSERT(0xffffffffffffeeebUL == result.data[0]);
  ASSERT(~(0UL) == result.data[1]);
  ASSERT(~(0UL) == result.data[2]);
  ASSERT(~(0UL) == result.data[3]);
}

void test_mul_rand_01() {
  // genfact.rb generated random test
  // 91e4f81cb57d39176204a55bde6b236 * 4fb8b92d5acc32f073a98a00f5bb277
  // = 2d6eeeab80ef518a350a27cac5e48b088f5433c2e0ebfb5388a3183158631a
  UInt256 left, right, result;
  left.data[0] = 0x76204a55bde6b236UL;
  left.data[1] = 0x91e4f81cb57d391UL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;

  right.data[0] = 0x073a98a00f5bb277UL;
  right.data[1] = 0x4fb8b92d5acc32fUL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;

  result = uint256_mul(left, right);

  ASSERT(0x5388a3183158631aUL == result.data[0]);
  ASSERT(0x088f5433c2e0ebfbUL == result.data[1]);
  ASSERT(0x8a350a27cac5e48bUL == result.data[2]);
  ASSERT(0x2d6eeeab80ef51UL == result.data[3]);
}

void test_mul_rand_02() {
  // genfact.rb generated random test
  // c5ad1fca1ec88b837455ea120125a8c * 39f0bfcea33d3d40235ed9ddf969722 =
  // 2cbd727d886b6979818994de0c192da8f8ae19439efd188f7186ce17e09a98
  UInt256 left, right, result;
  left.data[0] = 0x37455ea120125a8cUL;
  left.data[1] = 0xc5ad1fca1ec88b8UL;
  left.data[2] = 0x0UL;
  left.data[3] = 0x0UL;

  right.data[0] = 0x0235ed9ddf969722UL;
  right.data[1] = 0x39f0bfcea33d3d4UL;
  right.data[2] = 0x0UL;
  right.data[3] = 0x0UL;

  result = uint256_mul(left, right);

  ASSERT(0x8f7186ce17e09a98UL == result.data[0]);
  ASSERT(0xa8f8ae19439efd18UL == result.data[1]);
  ASSERT(0x79818994de0c192dUL == result.data[2]);
  ASSERT(0x2cbd727d886b69UL == result.data[3]);
}

void test_mul_zero() {
  // test num * 0 = 0
  UInt256 zero = uint256_create_from_u64(0UL);
  UInt256 left, result;
  left.data[0] = 0xae7d10a7ee8aa019UL;
  left.data[1] = 0xb9c17e8d8dd0082dUL;
  left.data[2] = 0xf66e444210c88f90UL;
  left.data[3] = 0xcbe26547255b3e3UL;
  result = uint256_mul(left, zero);
  for (int i = 0; i < 4; ++i) {
    ASSERT(0UL == result.data[i]);
  }
}

void test_mul_one() {
  // test num * 1 = num
  UInt256 one = uint256_create_from_u64(1UL);
  UInt256 left, result;
  left.data[0] = 0xe1e438bf984a4906UL;
  left.data[1] = 0xcdb140ecca492985UL;
  left.data[2] = 0xfc3588d6f1f3c1dfUL;
  left.data[3] = 0x43763d2d32248d2UL;
  result = uint256_mul(left, one);
  for (int i = 0; i < 4; ++i) {
    ASSERT(left.data[i] == result.data[i]);
  }
}

void test_mul_overload_01() {
  // aecfe8a9eaec157028b3f0708698fcb447853456855d62d82cb12abb0c6bc60 *
  // f8bd7febfbe6c62f500bc06efdf5d5ca75f67211cab9b7248c945784d090a6e =
  // c7106aeeb8a3e940 55bdd5f7c635f01e a80c70d5c59d3750 c37fe5533820b140
  UInt256 left, right, result;
  left.data[0] = 0x82cb12abb0c6bc60UL;
  left.data[1] = 0x447853456855d62dUL;
  left.data[2] = 0x028b3f0708698fcbUL;
  left.data[3] = 0xaecfe8a9eaec157UL;

  right.data[0] = 0x48c945784d090a6eUL;
  right.data[1] = 0xa75f67211cab9b72UL;
  right.data[2] = 0xf500bc06efdf5d5cUL;
  right.data[3] = 0xf8bd7febfbe6c62UL;

  result = uint256_mul(left, right);

  ASSERT(0xc37fe5533820b140UL == result.data[0]);
  ASSERT(0xa80c70d5c59d3750UL == result.data[1]);
  ASSERT(0x55bdd5f7c635f01eUL == result.data[2]);
  ASSERT(0xc7106aeeb8a3e940UL == result.data[3]);
}

void test_mul_overload_02() {
  // 8fa894b7091a5567bbfb60e5e395cf3f9c0f264a814cc95fe6a0ee7b9268485 *
  // e4bcaa53447025fd0322620c797b82f17172b07fd042afba0a9f8736a35df18 =
  // dd6bd08cf36fa934 64acb1aae8a088cc 4887d1d266eba7ec 5e0be7996a954778
  UInt256 left, right, result;
  left.data[0] = 0xfe6a0ee7b9268485UL;
  left.data[1] = 0xf9c0f264a814cc95UL;
  left.data[2] = 0x7bbfb60e5e395cf3UL;
  left.data[3] = 0x8fa894b7091a556UL;

  right.data[0] = 0xa0a9f8736a35df18UL;
  right.data[1] = 0x17172b07fd042afbUL;
  right.data[2] = 0xd0322620c797b82fUL;
  right.data[3] = 0xe4bcaa53447025fUL;

  result = uint256_mul(left, right);

  ASSERT(0x5e0be7996a954778UL == result.data[0]);
  ASSERT(0x4887d1d266eba7ecUL == result.data[1]);
  ASSERT(0x64acb1aae8a088ccUL == result.data[2]);
  ASSERT(0xdd6bd08cf36fa934UL == result.data[3]);
}