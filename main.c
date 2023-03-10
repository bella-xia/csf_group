/*
 * the main method that checks input validity(using input_check.c/h) and simulates a cache(using cache.c/h)
 * CSF Assignment 3
 * Richard Zhang
 * rzhang89@jhu.edu
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "input_check.h"
#include "cache.h"

int main(int argc, char* argv[]) {
    if (!check_inputs(argc,argv)) {//if input check fails, terminate the program and return 1
        return 1;
    }

    //initiate cache
    struct Cache cache = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'n','n','n',NULL,NULL,NULL};
    initiate_cache(&cache,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),argv[4],argv[5],argv[6]);

    char type; // record if we are doing a store or a load
    uint32_t command; //record the current input string
    char trash; //where useless inputs are dumped(not dump into NULL since it will cause warnings)

    while (scanf(" %c", &type) == 1) {
        scanf(" %c%c", &trash, &trash); //discard 0x(assuming that 0x exists)
        scanf(" %x",&command); //reads the hexadecimal into command
        switch (type) {
            case 's': // "store"
                store(&cache,command);
            break;

            case 'l': // "load"
                load(&cache,command);
            break;

            default:
            fprintf(stderr,"error: invalid command(neither load nor store) inputted\n");
            return 1; //return error
        }
        scanf(" %u", &command); //basically we scan the last input and ignores it
  }
    print_results(&cache); //print the results after everything is done
    free_cache(&cache); //free memory allocations in the end
    return 0;
}
