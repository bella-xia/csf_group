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
#define handle_error(msg)                                                      \
  do {                                                                         \
    fprintf(stderr, msg);                                                      \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void merge(int64_t *arr, size_t begin, size_t mid, size_t end,
           int64_t *temparr);
void sequential_merge_sort(int64_t *arr, size_t size);
void merge_sort(int64_t *arr, size_t begin, size_t end, size_t threshold);
int compare_i64(const void *a, const void *b) {
  int64_t left = *(int64_t *)a;
  int64_t right = *(int64_t *)b;
  if (left < right) {
    return -1;
  } else if (left == right) {
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

void sequential_merge_sort(int64_t *arr, size_t length) {
  qsort(arr, length, sizeof(int64_t), compare_i64);
}

int parallel_merge_sort(int64_t *arr, size_t begin, size_t end,
                        size_t threshold) {
  size_t length = end - begin;
  if (length > threshold) {
    size_t mid = (begin + end) / 2;
    int64_t *temp_arr = malloc(sizeof(int64_t) * length);
    pid_t pid = fork();
    if (pid == -1) {
      handle_error("Error: fail to start a new process.\n");
    }

    /* in child process*/
    else if (pid == 0) {
      int retcode_child = parallel_merge_sort(arr, begin, mid, threshold);
      exit(retcode_child);
    }
    int retcode_parent = parallel_merge_sort(arr, mid, end, threshold);
    /* outside child process*/
    int wstatus;
    pid_t actual_pid = waitpid(pid, &wstatus, 0);
    if (actual_pid == -1) {
      handle_error("Error: waitpid failure.\n");
    }
    if (!WIFEXITED(wstatus)) {
      handle_error("Error: subprocess not exit properly.\n");
    }
    if (WEXITSTATUS(wstatus) != 0) {
      handle_error("Error: subprocess returns a non-zero value.\n");
    }

    /*if child process end correctly*/
    else {
      merge(arr, begin, mid, end, temp_arr);
      for (int i = 0; i < length; i++) {
        arr[begin + i] = temp_arr[i];
      }
      free(temp_arr);
      return (retcode_parent);
    }
  } else {
    sequential_merge_sort(arr + begin, end - begin);
  }
  return (0);
}

int main(int argc, char **argv) {
  // check for correct number of command line arguments
  if (argc != 3) {
    fprintf(stderr, "Error: Usage: %s <filename> <sequential threshold>\n",
            argv[0]);
    return 1;
  }
  // process command line arguments
  const char *filename = argv[1];
  char *end;
  size_t threshold = (size_t)strtoul(argv[2], &end, 10);
  if (end != argv[2] + strlen(argv[2]) || threshold < 0) {
    fprintf(stderr, "Error: Invalid threshold\n");
    return 2;
  }
  /* TODO: report an error (threshold value is invalid) */;
  int fd = open(filename, O_RDWR);

  if (fd < 0) {
    fprintf(stderr, "Error: %s cannot be opened.\n", argv[1]);
    return 3;
  }
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    fprintf(stderr, "Error: fstat error.\n");
    return 4;
  }
  size_t file_size_in_bytes = statbuf.st_size;
  size_t size = file_size_in_bytes / sizeof(int64_t);
  int64_t *data =
      mmap(NULL, file_size_in_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (data == MAP_FAILED) {
    handle_error("Error: mmap error.\n");
    return 5;
  }

  int dead_child_comdition = parallel_merge_sort(data, 0, size, threshold);

  write(fd, data, file_size_in_bytes);
  close(fd);
  munmap(data, rc);
  return dead_child_comdition;

  // TODO: open the file

  // TODO: use fstat to determine the size of the file

  // TODO: map the file into memory using mmap

  // TODO: sort the data!

  // TODO: unmap and close the file

  // TODO: exit with a 0 exit code if sort was successful
}
