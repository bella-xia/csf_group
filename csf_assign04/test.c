#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void merge(int *arr, size_t begin, size_t mid, size_t end, int *temparr);
void sequential_merge_sort(int *arr, size_t begin, size_t end);
void merge_sort(int *arr, size_t begin, size_t end, size_t threshold);
int compare_i64(int* a, int* b) {
  return *a - *b;
}
// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
void merge(int *arr, size_t begin, size_t mid, size_t end, int *temparr) {
  int *endl = arr + mid;
  int *endr = arr + end;
  int *left = arr + begin, *right = arr + mid, *dst = temparr;

  for (;;) {
    int at_end_l = left >= endl;
    int at_end_r = right >= endr;

    if (at_end_l && at_end_r) break;

    if (at_end_l)
      *dst++ = *right++;
    else if (at_end_r)
      *dst++ = *left++;
    else {
      int cmp = compare_i64(left, right);
      if (cmp <= 0)
        *dst++ = *left++;
      else
        *dst++ = *right++;
    }
  }
}

void sequential_merge_sort(int *arr, size_t begin, size_t end) {
  size_t length = end - begin;
  if (begin < end - 1) {
    size_t mid = (begin + end) / 2;
    int* temp_arr = malloc(sizeof(int) * length);
    sequential_merge_sort(arr, begin, mid);
    sequential_merge_sort(arr, mid, end);
    merge(arr, begin, mid, end, temp_arr);
    for(int i = 0; i < length; i++) {
      arr[begin + i] = temp_arr[i];
    }
    free(temp_arr);
  }
}

void merge_sort(int *arr, size_t begin, size_t end, size_t threshold) {
  size_t length = end - begin;
  if (length <= threshold) {
    sequential_merge_sort(arr, begin, end);
  }
}


int main() {
    int arr[21] = {1, 0, 5,55,1000 ,2,10,20,30,40,50,2,7,19,7,2,4,5, 4, 3, 2};

    printf("original: ");
    for(int i = 0; i <21; i++) {
        printf("%d ", arr[i]);
        printf(" ");
    }
    printf(" hahah\n");
    merge_sort(arr, 0, 21, 100);
    printf("result: ");
    for(int i = 0; i < 21; i++) {
        printf("%d ", arr[i]);
        printf(" ");
    }
    printf("\n");
    return 0;
}