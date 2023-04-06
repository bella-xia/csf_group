#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <inttypes.h>
#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
   fprintf(stderr, "errrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrorrrrrrrrrrrrrrrrrrrrrrr");                                     \
    exit(EXIT_FAILURE);                                                        \
  } while (0);

void merge(int64_t *arr, size_t begin, size_t mid, size_t end,
           int64_t *temparr);
void sequential_merge_sort(int64_t *arr, size_t begin, size_t end);
void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold);
int compare_i64(int64_t *a, int64_t *b) {
  if (*a < *b) {
    return -1;
  } else if (*a == *b) {
    return 0;
  }
  return 1;
}
// Merge the elements in the sorted ranges [begin, mid) and [mid, end),
// copying the result into temparr.
void merge(int64_t *arr, size_t begin, size_t mid, size_t end,
           int64_t *temparr) {
  int64_t *endl = arr + mid;
  int64_t *endr = arr + end;
  int64_t *left = arr + begin, *right = arr + mid, *dst = temparr;

  for (;;) {
    int at_end_l = left >= endl;
    int at_end_r = right >= endr;

    if (at_end_l && at_end_r)
      break;

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

void sequential_merge_sort(int64_t *arr, size_t begin, size_t end) {
  size_t length = end - begin;
  if (begin < end - 1) {
    size_t mid = (begin + end) / 2;
    int64_t *temp_arr = malloc(sizeof(int64_t) * length);
    sequential_merge_sort(arr, begin, mid);
    sequential_merge_sort(arr, mid, end);
    merge(arr, begin, mid, end, temp_arr);
    for (int i = 0; i < length; i++) {
      arr[begin + i] = temp_arr[i];
    }
    free(temp_arr);
  }
}
int parallel_merge_sort(int64_t *arr, size_t begin, size_t end) {
  size_t length = end - begin;
  if (length > 1) {
    size_t mid = (begin + end) / 2;
    int64_t *temp_arr = malloc(sizeof(int64_t) * length);
    pid_t pid = fork();
     if(pid == -1) {
      handle_error("fail to start a new process.\n");
     } 
     
     /* in child process*/ 
     else if (pid == 0) {
      int retcode_child = parallel_merge_sort(arr, begin, mid);
      exit(retcode_child);
     }
     int retcode_parent = parallel_merge_sort(arr, mid, end);
     /* outside child process*/
     int wstatus;
     pid_t actual_pid = waitpid(pid, &wstatus, 0);
     if (actual_pid == -1) {
      handle_error("waitpid failure.\n");
     }
     if (!WIFEXITED(wstatus)) {
      handle_error("subprocess not exit properly.\n");
     }
     if (WEXITSTATUS(wstatus) != 0) {
      handle_error("subprocess returns a non-zero value.\n");
     } 
     
     /*if child process end correctly*/
     else {
      //int retcode_parent = parallel_merge_sort(arr, begin, mid);
      merge(arr, begin, mid, end, temp_arr);
                      printf("begin: %ld, mid %ld, end %ld\n", begin, mid, end);
                      printf("left half arr:\n");
                      for(int i = begin; i < mid; i++) {
                        printf("%" PRId64 " ", arr[i]);
                      }
                      printf("\n");
                      printf("right half arr:\n");
                      for(int i = mid; i < end; i++) {
                        printf("%" PRId64 " ", arr[i]);
                      }
                      printf("\n");
      for (int i = 0; i < length; i++) {
        arr[begin + i] = temp_arr[i];
      }
                      printf("after merge: begin: %ld, mid %ld, end %ld\n", begin, mid, end);
                      for(int i = 0; i < 10; i++) {
                        printf("%" PRId64 " ", arr[i]);
                      }
                      printf("\n");
      free(temp_arr);
      exit(retcode_parent);
     }
  }
  return(0);
}
void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold) {
                      printf("original arr:\n");
                      for(int i = begin; i < end; i++) {
                        printf("%" PRId64 " ", arr[i]);
                      }
                      printf("\n");
  size_t length = end - begin;
  if (length <= threshold) {
    sequential_merge_sort(arr, begin, end);
  } else {
    parallel_merge_sort(arr, begin, end);
  }
                      printf("resulted arr:\n");
                      for(int i = begin; i < end; i++) {
                        printf("%" PRId64 " ", arr[i]);
                      }
                      printf("\n");
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
  size_t threshold = (size_t)strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2]) || threshold < 0) {
    fprintf(stderr, "Invalid threshold\n");
    return 2;
  }
  /* TODO: report an error (threshold value is invalid) */;
  int fd = open(filename, O_RDWR);

  if (fd < 0) {
    fprintf(stderr, "%s cannot be opened.\n", argv[1]);
    return 3;
  }
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    fprintf(stderr, "fstat error.\n");
    return 4;
  }
  size_t file_size_in_bytes = statbuf.st_size;
  size_t size = file_size_in_bytes / sizeof(int64_t);
  int64_t *data =
      mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (data == MAP_FAILED) {
    handle_error("mmap");
    fprintf(stderr, "mmap error.\n");
    return 5;
  }

  merge_sort(data, 0, size, threshold);

  // FILE *fileToWrite = fopen("test.txt", "w");

  // fwrite(data, sizeof(int64_t), size, fileToWrite);
  write(fd, data, file_size_in_bytes);
  close(fd);
  munmap(data, rc);
  // fclose(fileToWrite);
  return 0;

  // TODO: open the file

  // TODO: use fstat to determine the size of the file

  // TODO: map the file into memory using mmap

  // TODO: sort the data!

  // TODO: unmap and close the file

  // TODO: exit with a 0 exit code if sort was successful
}
