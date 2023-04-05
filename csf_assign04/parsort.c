#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
void merge(int64_t *arr, size_t begin, size_t mid, size_t end, int64_t *temparr) {
  int64_t *endl = arr + mid;
  int64_t *endr = arr + end;
  int64_t *left = arr + begin, *right = arr + mid, *dst = temparr;

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

void sequential_merge_sort(int64_t *arr, size_t begin, size_t end, int64_t * temp_arr) {
  while (begin < end) {
    size_t mid = (begin + end) / 2;
    sequential_merge_sort(temp_arr, begin, mid, arr);
    sequential_merge_sort(temp_arr, mid+1, end, arr);
    merge(arr, begin, mid, end, temp_arr);
  }
}

void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
  size_t length = end - begin;
  if (length <= threshold) {
    int64_t* temp_arr = malloc(sizeof(int64_t) * length);
    for(int i = 0; i < length; i++ ) {
      temp_arr[i] = arr[i];
    }
    sequential_merge_sort(arr, begin, end, temp_arr);
    if ((int)ceil(log2(length)) % 2 == 1) {
      arr = temp_arr;
    }
  }
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <filename> <sequential threshold>\n", argv[0]);
    return 1;
  }

  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t) strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2]) || threshold < 0) {
    fprintf(stderr, "Invalid threshold\n");
    return 2;
  }
    /* TODO: report an error (threshold value is invalid) */;
  int fd = open(filename, O_RDWR);
  
  if (fd < 0) {
    fprintf(stderr, "%s cannot be opened.\n", argv[0]);
    return 3;
  }
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    fprintf(stderr, "fstat error.\n");
    return 4;
  }
  size_t file_size_in_bytes = statbuf.st_size;
  int64_t *data = mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
  close(fd);
  
  if (data == MAP_FAILED) {
    handle_error("mmap");
    fprintf(stderr, "mmap error.\n");
    return 5;
  }
  merge_sort(data, 0, rc, threshold);
  munmap(data, rc);
  return 0;

  // TODO: open the file
  
  // TODO: use fstat to determine the size of the file

  // TODO: map the file into memory using mmap

  // TODO: sort the data!

  // TODO: unmap and close the file

  // TODO: exit with a 0 exit code if sort was successful
}
